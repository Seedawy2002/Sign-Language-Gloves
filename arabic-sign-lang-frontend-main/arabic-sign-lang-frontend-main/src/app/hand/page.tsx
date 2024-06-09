'use client';

import { ChevronLeft, Volume2 } from 'lucide-react';
import Link from 'next/link';
import { useState } from 'react';

export default function Hand() {
    const [currentLetter, setCurrentLetter] = useState('');
    const [currentWord, setCurrentWord] = useState('');
    const [isLoading, setIsLoading] = useState(false);

    // const capture = useCallback(async () => {
    //     const imageSrc = (webcamRef.current as any).getScreenshot();
    //     setImageUrl(imageSrc);
    //     fetch(imageSrc)
    //         .then((res) => res.blob())
    //         .then((blob) => {
    //             const formData = new FormData();
    //             formData.append('image', blob, 'image.jpg');
    //             fetch('https://www.youssef.software/predict_img', {
    //                 method: 'POST',
    //                 body: formData,
    //             })
    //                 .then((res) => res.json())
    //                 .then((data) => {
    //                     console.log(data);
    //                     setCurrentLetter(data.result);
    //                     setCurrentWord((prev) => prev + data.result);
    //                 });
    //         });
    // }, [webcamRef]);

    const capture = async () => {
        setIsLoading(true);
        const res = await fetch('https://www.youssef.software/predict_hand', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
        });
        const data = await res.json();
        setCurrentLetter(data.result);
        setCurrentWord((prev) => prev + data.result);
        setIsLoading(false);
    };

    const handleSpeechCall = async () => {
        if (!currentWord || currentWord === '') return;

        const res = await fetch('https://youssef.software/say', {
            method: 'POST',
            body: JSON.stringify({ text: currentWord }),
            headers: {
                'Content-Type': 'application/json',
            },
        });
        // data is mp3
        const data = await res.blob();
        // play the audio
        const audio = new Audio(URL.createObjectURL(data));
        audio.play();
    };
    return (
        <main className="flex min-h-screen flex-col items-center space-y-8 p-10 max-w-lg mx-auto">
            <Link href="/" className="cursor-pointer self-start flex gap-1">
                <ChevronLeft />
                Back
            </Link>
            <div className="space-y-1">
                <h1 className="text-3xl font-bold text-center">
                    Arabic Sign Language
                </h1>
                <p className=" text-center text-gray-500">
                    Adjust your hand with the glove then capture on each letter
                </p>
            </div>
            <div className="w-full">
                <button
                    className="bg-purple-500 text-white px-4 py-2 rounded-md font-bold w-full
                                    disabled:cursor-wait disabled:bg-purple-300 disabled:text-gray-500 mt-2"
                    onClick={capture}
                    disabled={isLoading}
                >
                    Capture
                </button>
                <div className="flex w-full items-center justify-between mt-2">
                    <button
                        className="bg-red-500 text-white px-4 py-2 rounded-md font-bold w-[25%]"
                        onClick={() => {
                            setCurrentWord('');
                        }}
                    >
                        Clear
                    </button>
                    <button
                        className="bg-yellow-500 text-white px-4 py-2 rounded-md font-bold w-[25%]"
                        onClick={() => {
                            setCurrentWord(currentWord + ' ');
                        }}
                    >
                        Space
                    </button>
                    <button
                        className="bg-blue-500 text-white px-4 py-2 rounded-md font-bold w-[40%]"
                        onClick={() => {
                            setCurrentWord(currentWord.slice(0, -1));
                        }}
                    >
                        Backspace
                    </button>
                </div>
            </div>

            <div className="flex flex-col items-center justify-center gap-8 relative">
                {currentWord && currentWord !== '' && (
                    <div className="flex items-center gap-2">
                        <h1 className="text-3xl font-bold whitespace-normal">
                            {currentWord}
                        </h1>
                        <Volume2
                            size={32}
                            onClick={handleSpeechCall}
                            className="cursor-pointer text-blue-500"
                        />
                    </div>
                )}
            </div>
            {/* <button
                    className="bg-blue-500 hover:bg-blue-700 text-white font-bold py-2 px-4 rounded"
                    onClick={capture}
                    disabled={isLoading}
                >
                    Capture
                </button> */}
        </main>
    );
}
