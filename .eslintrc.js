module.exports = {
  extends: [
    'eslint:recommended'
  ],
  parserOptions: {
    ecmaVersion: 2019,
  },
  env: {
    es6: true,
    node: true,
  },
  rules: {
    quotes: ['error', 'single', 'avoid-escape'],
    semi: ['error', 'always'],

    'no-console': 'off',
  },
};
