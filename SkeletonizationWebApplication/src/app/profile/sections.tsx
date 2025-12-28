import { type ReactElement } from "react";
import { Calendar } from "lucide-react";

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

const formatDate = (date?: string | Date | null) => {
  if (!date) return "N/A";
  const d = date instanceof Date ? date : new Date(date);
  if (Number.isNaN(d.getTime())) return "N/A";
  return d.toLocaleDateString("en-US", {
    year: "numeric",
    month: "long",
    day: "numeric"
  });
};

export const getProfileSections = (user: User): ProfileSection[] => [
  {
    title: "Account Information",
    icon: <Calendar className="h-5 w-5" />,
    fields: [
      {
        label: "Member Since",
        value: formatDate(user.createdAt),
        icon: <Calendar className="h-4 w-4 text-gray-400" />
      },
      {
        label: "Last Updated",
        value: formatDate(user.updatedAt),
        icon: <Calendar className="h-4 w-4 text-gray-400" />
      }
    ]
  }
];
