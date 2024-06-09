'use client';
import WebcamComponent from '@/components/Webcam';
import { Drawer, DrawerContent, DrawerTrigger } from '@/components/ui/drawer';
import { ChevronLeft, Video, Volume2 } from 'lucide-react';
import Image from 'next/image';
import Link from 'next/link';
import { useCallback, useEffect, useRef, useState } from 'react';
import PIC from '../../../public/Sign_Lang.jpg';

export default function Home() {
    const [showCamera, setShowCamera] = useState(false);
    const [imageUrl, setImageUrl] = useState<null | string>(null);
    const webcamRef = useRef(null);

    const [currentLetter, setCurrentLetter] = useState('');
    const [currentWord, setCurrentWord] = useState('');
    const [isLoading, setIsLoading] = useState(false);

    const capture = useCallback(async () => {
        setIsLoading(true);
        const imageSrc = (webcamRef.current as any).getScreenshot();
        // add delay
        await fetch(imageSrc)
            .then((res) => res.blob())
            .then(async (blob) => {
                const formData = new FormData();
                formData.append('image', blob, 'image.jpg');
                await fetch('https://www.youssef.software/predict_img', {
                    method: 'POST',
                    body: formData,
                })
                    .then((res) => res.json())
                    .then((data) => {
                        console.log(data);
                        setCurrentLetter(data.result);
                        setCurrentWord((prev) => prev + data.result);
                    });
            });
        setImageUrl(imageSrc);
        setIsLoading(false);
    }, [webcamRef]);

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
        <main className="flex flex-col items-center justify-between p-10">
            <Link href="/" className="cursor-pointer self-start flex gap-1">
                <ChevronLeft />
                Back
            </Link>
            <div>
                <div className="flex flex-col items-center justify-center gap-0">
                    <button
                        // className="bg-green-500 text-white px-4 py-2 rounded-md font-bold w-full
                        // rounded-b-none"
                        className="my-2 flex items-center gap-2 bg-teal-600 rounded-full px-2 py-1 text-white"
                        onClick={() => setShowCamera((prev) => !prev)}
                    >
                        <Video />
                        {showCamera
                            ? 'Tap to Stop Recording'
                            : 'Tap to Start Recording'}
                    </button>
                    {showCamera && (
                        <div>
                            <WebcamComponent webcamRef={webcamRef} />
                            <button
                                className="bg-purple-500 text-white px-4 py-2 rounded-md font-bold w-full
                                    disabled:cursor-wait disabled:bg-purple-300 disabled:text-gray-500 mt-2"
                                onClick={capture}
                                disabled={isLoading}
                            >
                                Capture
                            </button>
                            <div className="flex flex-wrap items-center justify-between mt-2">
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
                                        setCurrentWord(
                                            currentWord.slice(0, -1)
                                        );
                                    }}
                                >
                                    Backspace
                                </button>
                            </div>
                        </div>
                    )}
                    {showCamera && imageUrl && (
                        <div className="flex flex-col items-center justify-center relative">
                            {/* Word */}
                            {currentWord && currentWord !== '' && (
                                <div className="flex items-center gap-2 my-3">
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
                            <h5 className="text-sm font-extralight">
                                Latest Prediction
                            </h5>
                            <div className="relative">
                                <Image
                                    src={imageUrl}
                                    alt="caputured Image"
                                    // className="h-16 w-16"
                                    width={150}
                                    height={150}
                                />
                                <h1 className="text-xl font-bold text-blue-500 absolute top-0 right-0 bg-white px-1">
                                    {isLoading
                                        ? ''
                                        : currentLetter != ''
                                        ? currentLetter
                                        : 'x'}
                                </h1>
                            </div>
                        </div>
                    )}
                </div>
            </div>
            {/* Sticky help Icon bottom right */}
            <Drawer>
                <DrawerTrigger>
                    <p className="fixed bottom-5 right-5 bg-blue-500 text-white px-4 py-2 rounded-full">
                        ?
                    </p>
                    {/* <div className="fixed bottom-5 right-5">
                        <button className="bg-blue-500 text-white px-4 py-2 rounded-full">
                            ?
                        </button>
                    </div> */}
                </DrawerTrigger>
                <DrawerContent>
                    <div className="w-full">
                        <Image
                            alt="sign lang"
                            src={PIC}
                            className="w-[90%] sm:w-[30%] p-4 mx-auto"
                        />
                    </div>
                </DrawerContent>
            </Drawer>
        </main>
    );
}
