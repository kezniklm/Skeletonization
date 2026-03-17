/**
 * @file sections.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Builds profile section data models.
 * @description Produces presentational profile section metadata and formatted values derived from user account timestamps.
 */

import { type ReactElement } from "react";
import { Calendar } from "lucide-react";

import { formatDateInTimezone } from "@/lib/date-time";

type ProfileSectionField = {
  label: string;
  value: string;
  icon: ReactElement;
};

type ProfileSection = {
  title: string;
  icon: ReactElement;
  fields: ProfileSectionField[];
};

type User = {
  createdAt?: string | Date | null;
  updatedAt?: string | Date | null;
};

/**
 * @brief Generates profile section definitions for rendering.
 * @description Formats relevant user account dates and returns section data consumed by profile section components.
 * @param user User object containing profile date fields.
 * @param timezone Optional timezone for date formatting.
 * @returns Array of profile section configuration objects.
 */
export const getProfileSections = (user: User, timezone?: string | null): ProfileSection[] => [
  {
    title: "Account Information",
    icon: <Calendar className="h-5 w-5" />,
    fields: [
      {
        label: "Member Since",
        value: formatDateInTimezone(
          user.createdAt,
          timezone,
          { year: "numeric", month: "long", day: "numeric" },
          "en-US"
        ),
        icon: <Calendar className="h-4 w-4 text-gray-400" />
      },
      {
        label: "Last Updated",
        value: formatDateInTimezone(
          user.updatedAt,
          timezone,
          { year: "numeric", month: "long", day: "numeric" },
          "en-US"
        ),
        icon: <Calendar className="h-4 w-4 text-gray-400" />
      }
    ]
  }
];
