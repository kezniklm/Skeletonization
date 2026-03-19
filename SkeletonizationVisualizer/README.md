# Skeletonization Visualizer

Frontend visualization tool for exploring skeletonization benchmark outputs.

This application is built with Vite + React and is intended for interactive analysis of benchmark data produced by the Skeletonization project.

## Tech Stack

- Vite 6
- React 19 + TypeScript
- Tailwind CSS 4
- JSZip (data handling/export workflows)

## Prerequisites

- Node.js 20+
- npm 10+

## Quick Start

1. Install dependencies:

```bash
npm install
```

2. Start development server:

```bash
npm run dev
```

3. Open the URL shown by Vite (typically `http://localhost:5173`).

## Scripts

- `npm run dev` - run the Vite development server
- `npm run build` - run TypeScript build and create production bundle
- `npm run preview` - locally preview production build
- `npm run lint` - run ESLint
- `npm run prettier` - check formatting

## Data Input

- Default static data can be provided from `public/benchmark_data.json`.
- The app is structured for benchmark-oriented datasets and comparison flows.

## Build Output

The Vite config uses `vite-plugin-singlefile`, so production output is packaged as a single-file style artifact under `dist/` (along with static assets as needed by Vite).

## Project Layout

- `src/pages` - page-level views
- `src/features` - feature modules for visual analysis
- `src/components` - reusable UI components
- `src/contexts` - shared React context state
- `src/constants.ts` - UI/application configuration constants
- `src/types.ts` - shared TypeScript types
- `public/benchmark_data.json` - default benchmark dataset

## Related Workspace Modules

This visualizer complements:

- `SkeletonizationCLI` benchmark generation
- C++ core implementations (`SkeletonizationCore*`)
- `SkeletonizationWebApplication` for full-stack workflow

## Notes

- Keep benchmark JSON schema stable when extending the visualizer.
- Run `npm run lint` and `npm run prettier` before submitting changes.
