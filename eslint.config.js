// @ts-check

import eslint from "@eslint/js";
import globals from "globals";
import tseslint from "typescript-eslint";

export default tseslint.config(
    eslint.configs.recommended,
    ...tseslint.configs.recommendedTypeChecked,
    {
        ignores: ["**/build/**"],
    },
    {
        languageOptions: {
            globals: {
                ...globals.browser,
                ...globals.serviceworker,
            },
            parserOptions: {
                project: ["tsconfig.json"],
                tsconfigRootDir: import.meta.dirname,
            },
        },
    },
    {
        files: ["**/*.config.ts"],
        languageOptions: {
            globals: {
                ...globals.node,
            },
            parserOptions: {
                project: ["tsconfig.node.json"],
                tsconfigRootDir: import.meta.dirname,
            },
        },
    },
    {
        files: ["**/*.config.js"],
        languageOptions: {
            globals: {
                ...globals.node,
            },
        },
        ...tseslint.configs.disableTypeChecked,
    },
);
