'use client';

import * as React from 'react';
import { Moon, Sun } from 'lucide-react';
import { useTheme } from 'next-themes';

import { Button } from '@/components/ui/button';

export function ModeToggle() {
    const { setTheme, theme } = useTheme();

    return (
        <Button
            onClick={() =>
                // setTheme((theme) => (theme === 'dark' ? 'light' : 'dark'))
                setTheme(theme === 'dark' ? 'light' : 'dark')
            }
            className="fixed bottom-4 left-4 z-50 p-2 rounded-full bg-white dark:bg-gray-800 shadow-md dark:shadow-lg transition-all"
            variant="outline"
            size="icon"
        >
            <Sun className="h-[1.2rem] w-[1.2rem] rotate-0 scale-100 transition-all dark:-rotate-90 dark:scale-0" />
            <Moon className="absolute h-[1.2rem] w-[1.2rem] rotate-90 scale-0 transition-all dark:rotate-0 dark:scale-100" />
            <span className="sr-only">Toggle theme</span>
        </Button>
    );
}
