/**
 * @file next.config.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Defines Next.js runtime configuration.
 * @description Configures allowed image sources and compiler settings for the web application.
 * @version 1.0
 * @date 2026-03-16
 */

import type { NextConfig } from "next";

/** @brief Next.js configuration object for this project. */
const nextConfig: NextConfig = {
  images: {
    localPatterns: [
      {
        pathname: "/api/storage/object"
      },
      {
        pathname: "/uploads/**"
      }
    ],
    remotePatterns: [
      {
        protocol: "https",
        hostname: "lh3.googleusercontent.com",
        port: "",
        pathname: "/**"
      },
      {
        protocol: "https",
        hostname: "avatars.githubusercontent.com",
        port: "",
        pathname: "/u/**"
      }
    ]
  },
  reactCompiler: true
};

module.exports = nextConfig;
