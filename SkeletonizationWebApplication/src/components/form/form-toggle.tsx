import { type InputHTMLAttributes } from "react";
import { useFormContext, useController } from "react-hook-form";

type FormToggleProps = Omit<InputHTMLAttributes<HTMLInputElement>, "type" | "name"> & {
  name: string;
  label?: string;
  description?: string;
};

export const FormToggle = ({ name, label, description, ...props }: FormToggleProps) => {
  const {
    control,
    formState: { errors }
  } = useFormContext();

  const { field } = useController({ name, control });

  const error = errors[name]?.message as string | undefined;

  return (
    <div className="w-full">
      <label htmlFor={name} className="relative inline-flex cursor-pointer items-center">
        <input
          id={name}
          type="checkbox"
          className="peer sr-only"
          {...field}
          checked={Boolean(field.value)}
          onChange={(e) => field.onChange(e.target.checked)}
          {...props}
        />
        <div className="peer h-6 w-11 rounded-full bg-gray-200 peer-checked:bg-cyan-600 peer-focus:ring-2 peer-focus:ring-cyan-500/20 peer-disabled:cursor-not-allowed peer-disabled:opacity-50 after:absolute after:top-0.5 after:left-0.5 after:h-5 after:w-5 after:rounded-full after:border after:border-gray-300 after:bg-white after:transition-all after:content-[''] peer-checked:after:translate-x-full peer-checked:after:border-white dark:bg-gray-700 dark:peer-checked:bg-cyan-500 dark:after:border-gray-600" />
        {(label ?? description) && (
          <div className="ml-3">
            {label && <span className="text-sm font-medium text-gray-700 dark:text-gray-300">{label}</span>}
            {description && <p className="text-xs text-gray-500 dark:text-gray-400">{description}</p>}
          </div>
        )}
      </label>
      {error && <p className="mt-1 text-sm text-red-600 dark:text-red-400">{error}</p>}
    </div>
  );
};
