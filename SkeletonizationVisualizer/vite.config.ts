/**
 * @file vite.config.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Configures Visualizer Vite build pipeline.
 * @description Enables React support and single-file output packaging for benchmark visualizer deployment.
 * @version 1.0
 * @date 2026-03-16
 */

import react from '@vitejs/plugin-react'
import { defineConfig } from 'vite'
import { viteSingleFile } from 'vite-plugin-singlefile'

// https://vite.dev/config/
/**
 * @brief Exports Vite configuration for the visualizer application.
 * @returns Vite configuration object.
 */
export default defineConfig({
  plugins: [react(), viteSingleFile()],
})
