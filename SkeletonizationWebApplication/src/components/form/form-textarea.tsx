import { type TextareaHTMLAttributes } from "react";
import { useFormContext } from "react-hook-form";

type FormTextareaProps = TextareaHTMLAttributes<HTMLTextAreaElement> & {
  name: string;
  label?: string;
};

export const FormTextarea = ({ name, label, className = "", ...props }: FormTextareaProps) => {
  const {
    register,
    formState: { errors }
  } = useFormContext();

  const error = errors[name]?.message as string | undefined;

  return (
    <div className="w-full">
      {label && (
        <label htmlFor={name} className="mb-1 block text-sm font-medium text-gray-700 dark:text-gray-300">
          {label}
        </label>
      )}
      <textarea
        id={name}
        {...register(name)}
        className={`w-full rounded-lg border border-gray-300 bg-white px-3 py-2 text-sm text-gray-900 transition-colors placeholder:text-gray-400 hover:border-cyan-400 focus:border-cyan-500 focus:ring-2 focus:ring-cyan-500/20 focus:outline-none disabled:cursor-not-allowed disabled:opacity-50 dark:border-gray-600 dark:bg-gray-800 dark:text-gray-100 dark:placeholder:text-gray-500 dark:hover:border-cyan-500 ${
          error ? "border-red-500 focus:border-red-500 focus:ring-red-500/20" : ""
        } ${className}`}
        {...props}
      />
      {error && <p className="mt-1 text-sm text-red-600 dark:text-red-400">{error}</p>}
    </div>
  );
};
