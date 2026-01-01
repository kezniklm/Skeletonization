"use client";

import { Search } from "lucide-react";

import { Input } from "@/components/ui/input";

type SearchInputProps = {
  value: string;
  onChange: (value: string) => void;
  placeholder?: string;
};

export const SearchInput = ({ value, onChange, placeholder = "Search..." }: SearchInputProps) => (
  <div className="relative flex-1">
    <Search className="text-muted-foreground absolute top-1/2 left-3 h-4 w-4 -translate-y-1/2 xl:h-3.5 xl:w-3.5 2xl:h-4 2xl:w-4" />
    <Input
      type="text"
      placeholder={placeholder}
      value={value}
      onChange={(e) => onChange(e.target.value)}
      className="h-9 pl-9 text-sm xl:h-8 xl:text-xs 2xl:h-9 2xl:text-sm"
    />
  </div>
);
