const fs = require('fs');
const path = require('path');
const { parseMidi } = require('midi-file');
const { getRateFromNoteNumber, parseNoteNumber } = require('./note');
const { SOUND3CNT_H, SOUND4CNT_H, envelope } = require('./register');
const { saw } = require('./waveram');


class SongBuilder {
  /**
   * @typedef {Object} AdditionalTrackEventInfo
   * @property {number} trackIndex
   * @property {number | undefined} gbaChannel
   * @property {number} absoluteTime
   * @property {number | undefined} duration
   * @property {number | undefined} durationSecond
   * @property {MidiTrackEvent | undefined} noteOnEvent
   * @property {MidiTrackEvent | undefined} noteOffEvent
   *
   * @typedef {ReturnType<typeof parseMidi>} ParsedMidi
   * @typedef {ParsedMidi['tracks'][0][0]} ParsedMidiEvent
   *
   * @typedef {ParsedMidiEvent & AdditionalTrackEventInfo} MidiTrackEvent
   */

  /**
   * @typedef {Object} CommandEndOfTrack
   * @property {'endOfTrack'} type
   *
   * @typedef {Object} CommandLoopPoint
   * @property {'loopPoint'} type
   *
   * @typedef {Object} CommandRest
   * @property {'rest'} type
   * @property {number} rest
   *
   * @typedef {Object} CommandBPM
   * @property {'bpm'} type
   * @property {number} bpm
   *
   * @typedef {Object} CommandWaveRAM
   * @property {'waveRam'} type
   * @property {number[]} waveRam
   *
   * @typedef {Object} CommandNoteOnCh1
   * @property {'noteOn'} type
   * @property {1} channel
   * @property {boolean} useLength
   * @property {number} frequency `SOUND1CNT_X`に使われる値
   * @property {number} SOUND1CNT_H `SOUND1CNT_H`の値
   *
   * @typedef {Object} CommandNoteOnCh2
   * @property {'noteOn'} type
   * @property {2} channel
   * @property {boolean} useLength
   * @property {number} frequency `SOUND2CNT_H`に使われる値
   * @property {number} SOUND2CNT_L `SOUND2CNT_L`の値
   *
   * @typedef {Object} CommandNoteOnCh3
   * @property {'noteOn'} type
   * @property {3} channel
   * @property {boolean} useLength
   * @property {number} frequency `SOUND3CNT_X`に使われる値
   * @property {number} SOUND3CNT_H `SOUND3CNT_H`の値
   *
   * @typedef {Object} CommandNoteOnCh4
   * @property {'noteOn'} type
   * @property {4} channel
   * @property {boolean} useLength
   * @property {number} SOUND4CNT_L `SOUND4CNT_L`の値
   * @property {number} SOUND4CNT_H `SOUND4CNT_H`の下位1バイト
   *
   * @typedef {Object} CommandNoteOff
   * @property {'noteOff'} type
   * @property {number} channel
   *
   * @typedef {CommandEndOfTrack | CommandLoopPoint | CommandRest | CommandBPM | CommandWaveRAM | CommandNoteOnCh1 | CommandNoteOnCh2 | CommandNoteOnCh3 | CommandNoteOnCh4 | CommandNoteOff} Command
   */


  /**
   * @param {number} microsecondsPerBeat
   * @returns {number}
   */
  static microsecondsPerBeatToBPM(microsecondsPerBeat) {
    return Math.round(60000000 / microsecondsPerBeat);
  }


  /**
   * @param {Map<number, number>} map
   * @param {number} value
   */
  static updateUsageMap(map, value) {
    map.set(value, (map.get(value) || 0) + 1);
  }


  /**
   * @param {Map<number, number>} map
   * @param {number[]} values
   */
  static findMostUsedValue(map, values) {
    return values
      .map(value => [value, map.get(value) || 0])
      .sort((a, b) => b[1] - a[1])[0][0];
  }


  /**
   * トラックイベントの優先度を求める。
   * 値が小さいほど優先度が高い
   * @param {MidiTrackEvent} event
   * @returns {number}
   */
  static getEventPriority(event) {
    switch (event.type) {
      case 'setTempo':
        return 100;

      case 'text':
        return 200;

      case 'controller':
        return 300;

      case 'noteOff':
        return 1000;

      case 'noteOn':
        return 2000;

      case 'endOfTrack':
        return 10000;
    }

    console.warn(`unknown event type: ${event.type}`);

    return 500;
  }


  /**
   * @param {MidiTrackEvent[]} track
   * @param {string} type
   * @returns {MidiTrackEvent | null}
   */
  static getTrackEvent(track, type) {
    return track.find(event => event.type === type);
  }


  /**
   * @param {MidiTrackEvent[]} track
   * @returns {string | undefined}
   */
  static getTrackName(track) {
    const trackNameEvent = this.getTrackEvent(track, 'trackName');
    return trackNameEvent ? trackNameEvent.text : undefined;
  }


  /**
   * 最終的なコマンドに出力されないイベントか調べる
   * 出力されないイベントなら`true`
   * ただしCC111（ループポイント）やendOfTrackはその時刻で区切る必要があるため`false`
   * @param {MidiTrackEvent} event
   * @returns {boolean}
   */
  static isNoCommandEvent(event) {
    switch (event.type) {
      case 'noteOn':
        return false;

      case 'noteOff':
        return event.durationSecond <= SongBuilder.USE_LENGTH_THRESHOLD;

      case 'setTempo':
        return false;

      case 'endOfTrack':
        return false;

      case 'controller':
        switch (event.controllerType) {
          case 111:
            return false;
        }
        return true;

      case 'text':
        return true;
    }

    console.warn(`unknown event type: ${event.type}`);

    return true;
  }

  /**
   * 約数の Set を求める
   * @param {number} number 約数を求めたい数
   * @returns {Set<number>}
   */
  static calcDivisorSet(number) {
    /** @type {Set<number>} */
    const set = new Set();
    for (let i = 1; i <= number; i++) {
      if (number % i) {
        continue;
      }
      set.add(i);
    }
    return set;
  }


  /**
   * @param {number[]} data
   * @param {number} byte
   */
  static push1B(data, byte) {
    if (typeof byte !== 'number') {
      throw new Error(`byte is not a number: ${byte}`);
    }
    if (byte > 255) {
      throw new Error(`byte out of range: ${byte} (${data.length})`);
    }
    data.push(byte);
  }


  /**
   * @param {number[]} data
   * @param {number} word
   */
  static push2B(data, word) {
    if (typeof word !== 'number') {
      throw new Error(`word is not a number: ${word}`);
    }
    if (word > 65535) {
      throw new Error(`word out of range: ${word} (${data.length})`);
    }
    // little endian
    SongBuilder.push1B(data, word & 0xFF);
    SongBuilder.push1B(data, word >> 8);
  }


  /**
   * @param {number[]} data
   * @param {number} offset
   * @param {number} word
   */
  static set2B(data, offset, word) {
    const temp = [];
    SongBuilder.push2B(temp, word);
    data[offset] = temp[0];
    data[offset + 1] = temp[1];
  }


  /**
   * @param {ParsedMidi} parsedMidi
   * @returns {MidiTrackEvent[]}
   */
  static generateEvents(parsedMidi) {
    /** @type {number} */
    const { ticksPerBeat } = parsedMidi.header;

    /** @type {MidiTrackEvent[]} */
    let events = [];

    /** @type {MidiTrackEvent[]} */
    const conductorEvents = [];

    for (const [trackIndex, _trackEvents] of parsedMidi.tracks.entries()) {
      /** @type {MidiTrackEvent[]} */
      const trackEvents = _trackEvents.reduce((prev, event) => [...prev, {
        ...event,
        trackIndex,
        absoluteTime: (prev.length ? prev[prev.length - 1].absoluteTime : 0) + event.deltaTime,
      }], []);

      // テンポ設定等のイベントを先に抽出する
      const currentConductorEvents = trackEvents.filter(event =>
        event.type === 'setTempo'
      );
      conductorEvents.push(...currentConductorEvents);

      // GBAのサウンドチャンネルに対応しないトラックならばスキップ
      const trackName = SongBuilder.getTrackName(trackEvents);
      if (!trackName || !/^CH[1-4]$/.test(trackName)) {
        continue;
      }

      // 対応するサウンドチャンネルを取得
      const gbaChannel = parseInt(trackName.substr(2), 10);

      // イベントにgbaChannelを追加
      for (const event of trackEvents) {
        event.gbaChannel = gbaChannel;
      }

      // noteOn、noteOffイベントにdurationや対応するイベントへの参照を追加
      for (const [index, noteOnEvent] of trackEvents.entries()) {
        if (noteOnEvent.type !== 'noteOn') {
          continue;
        }
        const noteOffEventIndex = trackEvents.slice(index).findIndex(event =>
          event.type === 'noteOff' &&
          event.channel === noteOnEvent.channel &&
          event.noteNumber === noteOnEvent.noteNumber
        );
        if (noteOffEventIndex < 0) {
          throw new Error('no noteOff event');
        }
        const eventsBetweenNote = trackEvents.slice(index + 1, index + noteOffEventIndex);
        if (eventsBetweenNote.some(event => event.type === 'noteOn' || event.type === 'noteOff')) {
          throw new Error('polyphony is not allowed');
        }
        const noteOffEvent = trackEvents[index + noteOffEventIndex];
        const duration = noteOffEvent.absoluteTime - noteOnEvent.absoluteTime;

        noteOnEvent.duration = duration;
        noteOffEvent.duration = duration;

        noteOffEvent.noteOnEvent = noteOnEvent;
        noteOnEvent.noteOffEvent = noteOffEvent;
      }

      events.push(...trackEvents.filter(event =>
        event.type === 'noteOn' ||
        event.type === 'noteOff' ||
        event.type === 'endOfTrack' ||
        (event.type === 'controller' && event.controllerType === 111) ||
        (event.type === 'text' && /^#@/.test(event.text))
      ));
    }

    // `conductorEvents`を結合
    // ただし`events`の最初のイベントより前のものは無視する
    const startTime = Math.min(...events.map(event => event.absoluteTime));
    events = events.concat(conductorEvents.filter(event => event.absoluteTime >= startTime));

    // CC111がない場合は先頭に追加
    if (!events.some(event => event.type === 'controller' && event.controllerType === 111)) {
      events.unshift({
        type: 'controller',
        controllerType: 111,
        trackIndex: -1,
        absoluteTime: startTime,
        deltaTime: 0,
      });
    }

    // 時間順にソート
    events.sort((a, b) =>
      (a.absoluteTime - b.absoluteTime) ||
      (SongBuilder.getEventPriority(a) - SongBuilder.getEventPriority(b))
    );

    // 2つ目以降のCC111を削除
    {
      const cc111Index = events.findIndex(event => event.type === 'controller' && event.controllerType === 111);
      events = events.filter((event, index) => event.type !== 'controller' || event.controllerType !== 111 || index === cc111Index);
    }

    // 最後以外のEOTを削除
    {
      const endOfTrackIndex = events.length - [...events].reverse().findIndex(event => event.type === 'endOfTrack') - 1;
      events = events.filter((event, index) => event.type !== 'endOfTrack' || index === endOfTrackIndex);
    }

    // 全 absoluteTime から開始時刻を引く
    for (const event of events) {
      event.absoluteTime -= startTime;
    }

    // durationSecondを追加
    {
      let bpm;
      for (const event of events) {
        if (event.type === 'setTempo') {
          bpm = SongBuilder.microsecondsPerBeatToBPM(event.microsecondsPerBeat);
          continue;
        }

        if (event.duration != null) {
          event.durationSecond = event.duration / ticksPerBeat * 60 / bpm;
        }
      }
    }

    return events;
  }


  /**
   * @param {MidiTrackEvent[]} events
   * @param {number} ticksPerBeat
   * @returns {number} `absoluteTime`のGCM
   */
  static calcTimeGCM(events, ticksPerBeat) {
    // absoluteTime の最大公約数を求める
    // ticksPerBeat の約数でもある数にしておく
    const divisorSet = SongBuilder.calcDivisorSet(ticksPerBeat);
    divisorSet.delete(1);
    for (const event of events) {
      if (SongBuilder.isNoCommandEvent(event)) {
        continue;
      }

      for (const divisor of divisorSet.values()) {
        if (event.absoluteTime % divisor) {
          divisorSet.delete(divisor);
          //console.log(event.absoluteTime, divisor, divisorSet, event);
        }
      }
    }
    divisorSet.add(1);

    return Math.max(...divisorSet.values());
  }


  /**
   * @param {MidiTrackEvent[]} events
   * @param {number} ticksPerBeat
   * @param {number} timeGcm
   * @returns {Command[]}
   */
  static generateCommands(events, ticksPerBeat, timeGcm) {
    /**
     * @typedef {Object} ChannelSpec
     * @property {number} pitch
     * @property {number} maxVelocity
     * @property {number} duty
     *
     * @typedef {Object} Channel4Spec
     * @property {number} r
     * @property {number} w
     * @property {number} s
     *
     * @typedef {Object} State
     * @property {number} bpm
     * @property {number[]} waveRam
     * @property {ChannelSpec[]} channelSpecs
     */

    /** @type {State} */
    const state = {
      bpm: null,
      waveRam: [],
      channelSpecs: (new Array(5)).fill(null).map(() => ({
        pitch: SongBuilder.DEFAULT_PITCH,
        maxVelocity: SongBuilder.DEFAULT_MAX_VELOCITY,
        duty: SongBuilder.DEFAULT_DUTY,
      })),
    };

    /** @type {State | null} */
    let loopPointState = null;

    /** @type {Map<string | number, Channel4Spec>} */
    const channel4SpecMap = new Map();

    /** @type {Command[]} */
    const commands = [];

    let prevTime = 0;
    for (const event of events) {
      if (!SongBuilder.isNoCommandEvent(event)) {
        // RESTを追加
        const deltaTime = (event.absoluteTime - prevTime) / timeGcm;
        prevTime = event.absoluteTime;

        if (deltaTime) {
          commands.push({
            type: 'rest',
            rest: deltaTime,
          });
        }
      }

      const channelSpec = event.gbaChannel ? state.channelSpecs[event.gbaChannel] : null;

      switch (event.type) {
        case 'endOfTrack':
          commands.push({
            type: 'endOfTrack',
          });
          break;

        case 'setTempo': {
          const bpm = SongBuilder.microsecondsPerBeatToBPM(event.microsecondsPerBeat);
          commands.push({
            type: 'bpm',
            bpm,
          });
          state.bpm = bpm;
          break;
        }

        case 'text': {
          const [, key, value] = event.text.match(/^#@([^=]+)=(.*)$/);
          switch (key) {
            case 'PITCH':
              channelSpec.pitch = parseInt(value, 10);
              break;

            case 'WAVERAM': {
              let waveRam;
              const [name, ...params] = value.split(',');
              switch (name) {
                case 'SAW':
                  waveRam = saw(parseInt(params[0], 10));
                  break;

                default:
                  throw new Error(`unknown WAVERAM type: ${name}`);
              }
              commands.push({
                type: 'waveRam',
                waveRam,
              });
              break;
            }

            case 'DUTY':
              channelSpec.duty = parseInt(value, 10);
              break;

            case 'CH4SPEC': {
              const [strNoteNumber, strParams] = value.split(':');
              const noteNumber = strNoteNumber === 'DEFAULT' ? -1 : parseNoteNumber(strNoteNumber);
              const params = strParams.split(',')
                .map(param => param.split('='))
                .reduce((acc, [key, value]) => ({
                  ...acc,
                  [key]: parseInt(value, 10),
                }), {});
              //console.log(strNoteNumber, noteNumber, strParams, params);
              channel4SpecMap.set(noteNumber, {
                r: params.r,
                w: params.w,
                s: params.s,
              });
              break;
            }
          }
          break;
        }

        case 'controller':
          switch (event.controllerType) {
            // CC111
            case 111:
              commands.push({
                type: 'loopPoint',
              });
              loopPointState = JSON.parse(JSON.stringify(state));
              break;
          }
          break;

        case 'noteOn': {
          const volumeCh3 = [
            [0, 0], // 0%
            [3, 0], // 25%
            [2, 0], // 50%
            [0, 1], // 75%
            [1, 0], // 100%
          ][Math.round((event.velocity / channelSpec.maxVelocity) * 4)];
          const useLength = event.durationSecond <= SongBuilder.USE_LENGTH_THRESHOLD;
          const length64 = useLength ? Math.round(64 - event.durationSecond * 256) : null;
          const length256 = useLength ? Math.round(256 - event.durationSecond * 256) : null;
          const velocity16 = Math.round(event.velocity / channelSpec.maxVelocity * 15);

          switch (event.gbaChannel) {
            case 1:
              commands.push({
                type: 'noteOn',
                channel: 1,
                useLength,
                frequency: getRateFromNoteNumber(event.noteNumber + channelSpec.pitch),
                SOUND1CNT_H: envelope(length64 || 0, channelSpec.duty, 0, 0, velocity16),
              });
              break;

            case 2:
              commands.push({
                type: 'noteOn',
                channel: 2,
                useLength,
                frequency: getRateFromNoteNumber(event.noteNumber + channelSpec.pitch),
                SOUND2CNT_L: envelope(length64 || 0, channelSpec.duty, 0, 0, velocity16),
              });
              break;

            case 3:
              commands.push({
                type: 'noteOn',
                channel: 3,
                useLength,
                frequency: getRateFromNoteNumber(event.noteNumber + channelSpec.pitch),
                SOUND3CNT_H: SOUND3CNT_H(length256 || 0, volumeCh3[0], volumeCh3[1]),
              });
              break;

            case 4: {
              const spec = channel4SpecMap.get(event.noteNumber) || channel4SpecMap.get(-1);
              if (!spec) {
                throw new Error(`channel 4 spec not defined for ${event.noteNumber}`);
              }
              commands.push({
                type: 'noteOn',
                channel: 4,
                useLength,
                SOUND4CNT_L: envelope(length64, 0, 0, 0, velocity16),
                SOUND4CNT_H: SOUND4CNT_H(spec.r, spec.w, spec.s),
              });
              break;
            }
          }
          break;
        }

        case 'noteOff':
          if (!SongBuilder.isNoCommandEvent(event)) {
            commands.push({
              type: 'noteOff',
              channel: event.gbaChannel,
            });
          }
          break;
      }
    }

    // 終了前にループポイントの状態を復帰
    if (loopPointState) {
      /** @type {Command[]} */
      const loopPointCommands = [];

      if (state.bpm !== loopPointState.bpm) {
        loopPointCommands.push({
          type: 'bpm',
          bpm: loopPointState.bpm,
        });
      }

      if (state.waveRam.some((v, i) => v !== loopPointState.waveRam[i])) {
        loopPointCommands.push({
          type: 'waveRam',
          waveRam: loopPointState.waveRam,
        });
      }

      commands.push(...loopPointCommands);
    }

    return commands;
  }


  /**
   * @param {Command[]} commands
   * @param {number} ticksPerBeat
   * @param {number} timeGcm
   * @returns {number[]}
   */
  static generateBinaryCommands(commands, ticksPerBeat, timeGcm) {
    /**
     * @typedef {Object} Table
     * @property {number[]} array
     * @property {Map<number, number>} map
     */

    /**
     * @param {number[]} array
     * @returns {Table}
     */
    function arrayToTable(array) {
      return {
        array,
        map: new Map(array.map((v, i) => [v, i])),
      };
    }

    /**
     * @param {Map<number, number>} usageMap
     * @param {number} max
     * @returns {Table}
     */
    function usageMapToTable(usageMap, max) {
      return arrayToTable(Array.from(usageMap.entries())
        .sort((a, b) => b[1] - a[1])
        .map(v => v[0])
        .slice(0, max)
        .sort((a, b) => a - b)
      );
    }

    // 各レジスタ値の使用量を求める

    /** @type {Set<number>} */
    const frequencySet = new Set();

    /** @type {Map<number, number>} */
    const envelopeUsageMap = new Map();

    /** @type {Map<number, number>} */
    const channel3ValueUsageMap = new Map();

    for (const command of commands) {
      if (command.type !== 'noteOn') {
        continue;
      }

      if (command.frequency) {
        frequencySet.add(command.frequency);
      }

      switch (command.channel) {
        case 1:
          SongBuilder.updateUsageMap(envelopeUsageMap, command.SOUND1CNT_H);
          break;

        case 2:
          SongBuilder.updateUsageMap(envelopeUsageMap, command.SOUND2CNT_L);
          break;

        case 3:
          SongBuilder.updateUsageMap(channel3ValueUsageMap, command.SOUND3CNT_H);
          break;

        case 4:
          SongBuilder.updateUsageMap(envelopeUsageMap, command.SOUND4CNT_L);
          break;
      }
    }

    const frequencyTable = arrayToTable(Array.from(frequencySet.values()).sort((a, b) => a - b));
    const envelopeTable = usageMapToTable(envelopeUsageMap);
    const channel3ValueTable = usageMapToTable(channel3ValueUsageMap);

    /*
    console.log(frequencyTable);
    console.log(envelopeUsageMap);
    console.log(envelopeTable);
    console.log(channel3ValueUsageMap);
    console.log(channel3ValueTable);
    //*/

    if (frequencyTable.array.length % 2) {
      frequencyTable.array.push(0);
    }

    if (envelopeTable.array.length % 2) {
      envelopeTable.array.push(0);
    }

    if (channel3ValueTable.array.length % 2) {
      channel3ValueTable.array.push(0);
    }


    // コマンド部ビルド

    /** @type {number[]} */
    const binaryCommands = [];

    let loopPointOffset = 0;

    for (const command of commands) {
      switch (command.type) {
        case 'endOfTrack':
          // do nothing
          break;

        case 'loopPoint':
          loopPointOffset = binaryCommands.length;
          break;

        case 'rest': {
          let restDeltaTime = command.rest;
          while (restDeltaTime) {
            if (restDeltaTime <= 128) {
              SongBuilder.push1B(binaryCommands, SongBuilder.COMMAND_ID.RESTSHORT_BASE + restDeltaTime - 1);
              restDeltaTime = 0;
              continue;
            }
            const rest = Math.min(restDeltaTime, 65535);
            SongBuilder.push1B(binaryCommands, SongBuilder.COMMAND_ID.RESTLONG);
            SongBuilder.push2B(binaryCommands, rest);
            restDeltaTime -= rest;
          }
          break;
        }

        case 'bpm':
          SongBuilder.push1B(binaryCommands, SongBuilder.COMMAND_ID.BPM);
          SongBuilder.push2B(binaryCommands, command.bpm);
          break;

        case 'waveRam':
          SongBuilder.push1B(binaryCommands, SongBuilder.COMMAND_ID.WAVERAM);
          // 1バイトずつ置く場合は上位4ビット→下位4ビットで置いていけば良い
          // （2バイトずつの場合リトルエンディアンなので2バイトずつ左右のバイトを反転して入れる必要がある）
          for (let i = 0; i < 16; i++) {
            SongBuilder.push1B(binaryCommands, (command.waveRam[i * 2] << 4) | command.waveRam[i * 2 + 1]);
          }
          break;

        case 'noteOn':
          switch (command.channel) {
            case 1: {
              const frequencyIndex = frequencyTable.map.get(command.frequency);
              const envelopeIndex = envelopeTable.map.get(command.SOUND1CNT_H);
              SongBuilder.push1B(binaryCommands, SongBuilder.COMMAND_ID.SOUND1ON_BASE | (envelopeIndex == null ? 0x01 : 0x00));
              if (envelopeIndex != null) {
                SongBuilder.push1B(binaryCommands, envelopeIndex);
              } else {
                SongBuilder.push2B(binaryCommands, command.SOUND1CNT_H);
              }
              SongBuilder.push1B(binaryCommands, frequencyIndex | (command.useLength ? 0x80 : 0x00));
              break;
            }

            case 2: {
              const frequencyIndex = frequencyTable.map.get(command.frequency);
              const envelopeIndex = envelopeTable.map.get(command.SOUND2CNT_L);
              SongBuilder.push1B(binaryCommands, SongBuilder.COMMAND_ID.SOUND2ON_BASE | (envelopeIndex == null ? 0x01 : 0x00));
              if (envelopeIndex != null) {
                SongBuilder.push1B(binaryCommands, envelopeIndex);
              } else {
                SongBuilder.push2B(binaryCommands, command.SOUND2CNT_L);
              }
              SongBuilder.push1B(binaryCommands, frequencyIndex | (command.useLength ? 0x80 : 0x00));
              break;
            }

            case 3: {
              const frequencyIndex = frequencyTable.map.get(command.frequency);
              const channel3ValueIndex = channel3ValueTable.map.get(command.SOUND3CNT_H);
              SongBuilder.push1B(binaryCommands, SongBuilder.COMMAND_ID.SOUND3ON_BASE | (channel3ValueIndex == null ? 0x01 : 0x00));
              if (channel3ValueIndex != null) {
                SongBuilder.push1B(binaryCommands, channel3ValueIndex);
              } else {
                SongBuilder.push2B(binaryCommands, command.SOUND3CNT_H);
              }
              SongBuilder.push1B(binaryCommands, frequencyIndex | (command.useLength ? 0x80 : 0x00));
              break;
            }

            case 4: {
              const envelopeIndex = envelopeTable.map.get(command.SOUND4CNT_L);
              SongBuilder.push1B(binaryCommands, SongBuilder.COMMAND_ID.SOUND4ON_BASE | (envelopeIndex == null ? 0x01 : 0x00) | (command.useLength ? 0x02 : 0x00));
              if (envelopeIndex != null) {
                SongBuilder.push1B(binaryCommands, envelopeIndex);
              } else {
                SongBuilder.push2B(binaryCommands, command.SOUND4CNT_L);
              }
              SongBuilder.push1B(binaryCommands, command.SOUND4CNT_H);
              break;
            }
          }
          break;

        case 'noteOff':
          SongBuilder.push1B(binaryCommands, [
            null,
            SongBuilder.COMMAND_ID.SOUND1OFF,
            SongBuilder.COMMAND_ID.SOUND2OFF,
            SongBuilder.COMMAND_ID.SOUND3OFF,
            SongBuilder.COMMAND_ID.SOUND4OFF,
          ][command.channel]);
          break;
      }
    }

    while (binaryCommands.length % 4) {
      SongBuilder.push1B(binaryCommands, SongBuilder.COMMAND_ID.NOP);
    }


    // ヘッダ部ビルド

    /** @type {number[]} */
    const header = [];

    SongBuilder.push2B(header, ticksPerBeat / timeGcm);
    SongBuilder.push2B(header, 0);    // filled later
    SongBuilder.push2B(header, 0);    // filled later
    SongBuilder.push2B(header, frequencyTable.array.length);
    SongBuilder.push2B(header, envelopeTable.array.length);
    SongBuilder.push2B(header, channel3ValueTable.array.length);

    for (const value of frequencyTable.array) {
      SongBuilder.push2B(header, value);
    }

    for (const value of envelopeTable.array) {
      SongBuilder.push2B(header, value);
    }

    for (const value of channel3ValueTable.array) {
      SongBuilder.push2B(header, value);
    }

    // `offsetToEnd`と`offsetToLoopPoint`を設定
    const commandDataOffset = header.length;
    SongBuilder.set2B(header, 2, commandDataOffset + binaryCommands.length);
    SongBuilder.set2B(header, 4, commandDataOffset + loopPointOffset);


    // 結合して返す
    return header.concat(binaryCommands);
  }


  /**
   * @param {string} midiFilepath
   */
  constructor(midiFilepath) {
    this.midiFilepath = midiFilepath;
    this.outPath = path.join(this.midiFilepath, '..', path.parse(this.midiFilepath).name + '.cpp');
    this.varName = path.parse(this.midiFilepath).name;

    this.bpm = null;
    this.loopPoint = 0;

    this.binaryMidiData = fs.readFileSync(this.midiFilepath);
    this.parsedMidi = parseMidi(this.binaryMidiData);
    this.ticksPerBeat = this.parsedMidi.header.ticksPerBeat;

    /*
    fs.writeFileSync(`${this.midiFilepath}.json`, JSON.stringify(this.parsedMidi, null, 2));
    throw 1;
    //*/

    this.events = SongBuilder.generateEvents(this.parsedMidi);
    this.timeGcm = SongBuilder.calcTimeGCM(this.events, this.ticksPerBeat);

    //console.log(this.ticksPerBeat, this.timeGcm);

    this.commands = SongBuilder.generateCommands(this.events, this.ticksPerBeat, this.timeGcm);
    this.binary = SongBuilder.generateBinaryCommands(this.commands, this.ticksPerBeat, this.timeGcm);

    while (this.binary.length % 4) {
      this.binary.push1B(0x00);
    }

    this.binaryBuffer = Buffer.from(this.binary);

    let str = `
#include <cstdint>

namespace Song {
  extern const std::uint32_t ${this.varName}[${this.binaryBuffer.length / 4}] = {
`.trim() + '\n    ';
    for (let i = 0; i < this.binaryBuffer.length / 4; i++) {
      str += '0x' + ('0000000' + this.binaryBuffer.readUInt32LE(i * 4).toString(16).toUpperCase()).substr(-8);
      str += i % 16 == 15 ? ',\n    ' : ', ';
    }
    str = str.trimRight() + '\n  };\n}\n';

    fs.writeFileSync(this.outPath, str);

    //fs.writeFileSync(`${this.outPath}.commands.json`, JSON.stringify(this.commands, null, 2));
    //fs.writeFileSync(`${this.outPath}.dat`, this.binaryBuffer);
  }
}

const bgmVolume = 1;

SongBuilder.DEFAULT_PITCH = 0;
SongBuilder.DEFAULT_MAX_VELOCITY = 127 / bgmVolume;
SongBuilder.DEFAULT_DUTY = 2;
SongBuilder.USE_LENGTH_THRESHOLD = 64 / 256;

SongBuilder.COMMAND_ID = {
  NOP: 0x00,
  RESTLONG: 0x01,
  BPM: 0x02,
  //
  SOUNDCNT_L: 0x04,
  SOUNDCNT_H: 0x05,
  SOUNDCNT_X: 0x06,
  SOUNDBIAS: 0x07,
  WAVERAM: 0x08,
  //
  SOUND1ON_BASE: 0x10,
  SOUND2ON_BASE: 0x14,
  SOUND3ON_BASE: 0x18,
  SOUND4ON_BASE: 0x1C,
  //
  SOUND1OFF: 0x20,
  SOUND2OFF: 0x21,
  SOUND3OFF: 0x22,
  SOUND4OFF: 0x23,
  //
  SETREG_BASE: 0x40,
  //
  RESTSHORT_BASE: 0x80,
};


if (!process.argv[2]) {
  console.log(`usage: node ${process.argv[1]} song.mid`);
  process.exit(1);
}

new SongBuilder(process.argv[2]);
