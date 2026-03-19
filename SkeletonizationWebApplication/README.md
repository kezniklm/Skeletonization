# Skeletonization Web Application

Full-stack web interface for the Skeletonization project, built with Next.js.

It provides UI flows for image preprocessing, skeletonization job submission, and result visualization, and integrates with shared project services (database, Redis queues, worker, and optional S3/R2 storage).

## Tech Stack

- Next.js 16 + React 19 + TypeScript
- Tailwind CSS 4
- Drizzle ORM + PostgreSQL
- Redis queues
- Better Auth (OAuth providers)
- Trigger.dev jobs

## Prerequisites

- Node.js 20+
- npm 10+
- PostgreSQL instance
- Redis instance
- (Optional) S3-compatible bucket (AWS S3 / Cloudflare R2)

## Quick Start

1. Install dependencies:

```bash
npm install
```

2. Create environment file from template:

```bash
cp .env-example .env
```

On Windows PowerShell:

```powershell
Copy-Item .env-example .env
```

3. Fill required values in `.env`.

4. Start development server:

```bash
npm run dev
```

5. Open `http://localhost:3000`.

## Environment Variables

Use `.env-example` as the source of truth.

### Storage

- `STORAGE_BACKEND` - `local` or `s3`
- `PUBLIC_UPLOADS_DIR` - local uploads directory name (when using local storage)
- `S3_ENDPOINT`
- `S3_REGION`
- `S3_BUCKET`
- `S3_ACCESS_KEY_ID`
- `S3_SECRET_ACCESS_KEY`
- `S3_FORCE_PATH_STYLE`

### Database

- `DATABASE_URL` - PostgreSQL connection string

### Redis / Queues

- `REDIS_URL`
- `JOBS_QUEUE`
- `RESULTS_QUEUE`

### Authentication

- `BETTER_AUTH_SECRET`
- `BETTER_AUTH_URL`
- `GOOGLE_CLIENT_ID`
- `GOOGLE_CLIENT_SECRET`
- `GITHUB_CLIENT_ID`
- `GITHUB_CLIENT_SECRET`
- `DISCORD_CLIENT_ID`
- `DISCORD_CLIENT_SECRET`

## Scripts

- `npm run dev` - start development server
- `npm run build` - production build
- `npm run start` - run production server
- `npm run lint` - run ESLint
- `npm run prettier` - check formatting

## Project Layout

- `src/app` - App Router pages, layouts, and route handlers
- `src/components` - UI components
- `src/server-actions` - server-side actions
- `src/repositories` - data-access layer
- `src/database` - database schemas and utilities
- `src/trigger` - Trigger.dev background jobs integration
- `docs/erd.puml` - data model diagram source

## Related Services in This Repository

This app is part of a multi-project workspace and typically works together with:

- `SkeletonizationWorker` for background processing
- core C++ modules for algorithm execution

## Notes

- Queue names in `.env` should match the worker configuration.
- If `STORAGE_BACKEND=s3`, S3 credentials and bucket settings are required.
- Keep secrets out of version control; commit only `.env-example`.
