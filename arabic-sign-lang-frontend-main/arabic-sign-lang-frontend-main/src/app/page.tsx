'use client';
import { Hand, Type, Video } from 'lucide-react';
import Image from 'next/image';
import Link from 'next/link';

export default function Home() {
    return (
        <main className="flex min-h-screen flex-col items-center justify-between p-24">
            {/* Design a home page with logo centered and 2 buttons for links, one for use glove, the other one is to use camera. Use tailwind styles to make it beautiful*/}
            <div className="flex flex-col items-center justify-center gap-4 text-center">
                <Image src="/logo.png" alt="logo" width={200} height={200} />
                <h1 className="text-xl font-bold">
                    Welcome to Arabic Sign Language Translator 👋🏻
                </h1>
                <p className="text-sm font-extralight">
                    Please choose a method to start translating
                </p>
                <div className="flex flex-col gap-5">
                    <Link
                        href={'/hand'}
                        className="bg-blue-500 hover:bg-blue-700 text-white font-bold py-2 px-4 rounded flex items-center gap-2"
                    >
                        <Hand />
                        Use Glove
                    </Link>
                    <Link
                        href="/cam"
                        className="bg-purple-500 hover:bg-purple-700 text-white font-bold py-2 px-4 rounded flex items-center gap-2"
                    >
                        <Video />
                        Use Camera
                    </Link>
                    <Link
                        href="/realtime"
                        className="bg-green-500 hover:bg-green-700 text-white font-bold py-2 px-4 rounded flex items-center gap-2"
                    >
                        <Type />
                        Realtime
                    </Link>
                </div>
            </div>
        </main>
    );
}
