#include <array>
#include <cstddef>
#include <cstdint>

// # Header (excluding Entry Point)
namespace Header {
  namespace Part {
    constexpr char Logo[] =
      // Nintendo Logo (compressed bitmap, required!)
      "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
      "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
      "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
      "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
      "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
      "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
      "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
      "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
      "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
      "\0\0\0\0\0\0\0\0\0\0\0\0";

    static_assert(sizeof(Logo) == 156 + 1);

    //

    constexpr char Header[] =
      // Game Title (uppercase ascii, max 12 characters)
      "TETRA GBA\0\0\0"
      // Game Code (uppercase ascii, 4 characters)
      "TGBA"
      // Maker Code (uppercase ascii, 2 characters)
      "SR"
      // Fixed value (must be 96h, required!)
      "\x96"
      // Main unit code (00h for current GBA models)
      "\0"
      // Device type (usually 00h) (bit7=DACS/debug related)
      "\0"
      // Reserved Area (should be zero filled)
      "\0\0\0\0\0\0\0"
      // Software version (usually 00h)
      "\0";

    static_assert(sizeof(Header) == 29 + 1);

    //

    constexpr std::uint_fast8_t CalcChecksum() {
      std::uint_fast8_t c = 0;
      for (std::size_t i = 0; i < 29; i++) {
        c -= Header[i];
      }
      c -= 0x19;
      return c & 0xFF;
    }

    constexpr auto Checksum = CalcChecksum();

    //

    constexpr char Appendix[] =
      // Reserved Area (should be zero filled)
      "\0\0";

    static_assert(sizeof(Appendix) == 2 + 1);
  } // namespace Part

  constexpr std::array<std::uint8_t, 188> CreateHeader() {
    std::array<std::uint8_t, 188> header{};

    std::size_t offset = 0;

    for (std::size_t i = 0; i < sizeof(Part::Logo) - 1; i++, offset++) {
      header[offset] = Part::Logo[i];
    }

    for (std::size_t i = 0; i < sizeof(Part::Header) - 1; i++, offset++) {
      header[offset] = Part::Header[i];
    }

    header[offset++] = Part::Checksum;

    for (std::size_t i = 0; i < sizeof(Part::Appendix) - 1; i++, offset++) {
      header[offset] = Part::Appendix[i];
    }

    return header;
  }

  constexpr auto Header = CreateHeader();
} // namespace Header

//

// # Multiboot Header (excluding Entry Point)
namespace MBHeader {
  namespace Part {
    constexpr char Header[] =
      // Boot mode (init as 00h - BIOS overwrites this value!)
      "\0"
      // Slave ID Number (init as 00h - BIOS overwrites this value!)
      "\0"
      // Not used (seems to be unused)
      "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
      "\0\0\0\0\0\0\0\0\0\0";

    static_assert(sizeof(Header) == 28 + 1);
  } // namespace Part

  constexpr std::array<std::uint8_t, 28> CreateHeader() {
    std::array<std::uint8_t, 28> header{};

    std::size_t offset = 0;

    for (std::size_t i = 0; i < sizeof(Part::Header) - 1; i++, offset++) {
      header[offset] = Part::Header[i];
    }

    return header;
  }

  constexpr auto Header = CreateHeader();
} // namespace MBHeader

//

const auto binary_header __attribute__((section("._binary_header.rom"))) __attribute__((used)) = Header::Header;
const auto binary_header_multiboot __attribute__((section("._binary_header.multiboot"))) __attribute__((used)) = MBHeader::Header;
