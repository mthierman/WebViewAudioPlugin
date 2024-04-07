import { resolve } from "node:path";
import { defineConfig } from "vite";

// https://vitejs.dev/config/
export default defineConfig({
    root: resolve("gui"),
    base: "/",
    build: {
        outDir: resolve("build/gui"),
        emptyOutDir: true,
        rollupOptions: {
            output: {
                entryFileNames: `assets/[name].js`,
                chunkFileNames: `assets/[name].js`,
                assetFileNames: `assets/[name].[ext]`,
            },
        },
    },
    resolve: {
        alias: {
            css: resolve("gui/css"),
        },
    },
});
