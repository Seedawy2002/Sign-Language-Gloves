'use client';
import { Camera, SwitchCamera } from 'lucide-react';
import React, { useState } from 'react';
import Webcam from 'react-webcam';

interface WebcamComponentType {
    webcamRef: React.RefObject<Webcam>;
}

function WebcamComponent({ webcamRef }: WebcamComponentType) {
    const [facingMode, setFacingMode] = useState<'user' | 'environment'>(
        'user'
    );

    const videoConstraints = {
        facingMode: facingMode,
    };

    const toggleFacingMode = () => {
        setFacingMode((prevFacingMode) =>
            prevFacingMode === 'user' ? 'environment' : 'user'
        );
    };

    return (
        <div
            style={{
                display: 'flex',
                flexDirection: 'row',
                alignItems: 'center',
                justifyContent: 'space-between',
                gap: '2rem',
            }}
        >
            <div className="flex flex-col items-center gap-4 relative">
                <SwitchCamera
                    className="absolute top-2 right-2 cursor-pointer z-50 "
                    onClick={toggleFacingMode}
                />

                <Webcam
                    audio={false}
                    height={500}
                    ref={webcamRef}
                    screenshotFormat="image/jpeg"
                    width={500}
                    videoConstraints={videoConstraints}
                />
            </div>
        </div>
    );
}

export default WebcamComponent;
