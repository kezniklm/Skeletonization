/**
 * @file page.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Renders landing page entry route.
 * @description Displays product branding and feature overview, then conditionally renders welcome panel or sign-in card.
 * @version 1.0
 * @date 2026-03-16
 */

import { headers } from "next/headers";

import { auth } from "@/auth";

import { landingPageFeatures } from "./branding/landing-page-features";
import { AlgorithmShowcase } from "./branding/algorithm-showcase";
import { Branding } from "./branding/branding";
import { Feature } from "./branding/feature";
import { WelcomeBack } from "./branding/welcome-back";
import { SignInCard } from "./sign-in/sign-in-card";

/**
 * @brief Renders landing page with authentication-aware right panel.
 * @returns Landing page layout JSX.
 */
const LandingPage = async () => {
  const session = await auth.api.getSession({ headers: await headers() });

  const isLoggedIn = !!session?.user;

  return (
    <div className="flex flex-col items-center justify-center px-4 sm:px-6 lg:flex-row lg:px-8 xl:px-10 2xl:px-8">
      <div className="mb-12 max-w-xl lg:mr-16 lg:mb-0 lg:w-1/2 xl:mr-10 xl:max-w-lg 2xl:mr-16 2xl:max-w-xl">
        <Branding />

        <section className="space-y-4 xl:space-y-3 2xl:space-y-4" aria-label="Key features">
          {landingPageFeatures.map((feature) => (
            <Feature key={feature.title} icon={feature.icon} title={feature.title} description={feature.description} />
          ))}
        </section>

        <AlgorithmShowcase />
      </div>

      {isLoggedIn ? (
        <WelcomeBack className="w-full max-w-md lg:w-auto xl:max-w-sm 2xl:max-w-md" userName={session.user.name} />
      ) : (
        <SignInCard />
      )}
    </div>
  );
};

export default LandingPage;
