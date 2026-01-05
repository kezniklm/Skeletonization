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
