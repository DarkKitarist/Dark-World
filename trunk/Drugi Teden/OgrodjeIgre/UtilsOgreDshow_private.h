/// Do not include this file directly, always use UtilsOgreDshow.h instead.
// Ogre Dshow: small wrapper for video reproduction in Ogre, using Direct Show 9.
/*
   Wrapper for video reproduction using Direct Show in the Ogre 3d engine.
 
   Coded by H. Hernán Moraldo from Moraldo Games
   www.hernan.moraldo.com.ar/pmenglish/field.php
 
   --------------------
 
   Copyright (c) 2007 Horacio Hernan Moraldo
 
   This software is provided 'as-is', without any express or
   implied warranty. In no event will the authors be held liable
   for any damages arising from the use of this software.
 
   Permission is granted to anyone to use this software for any
   purpose, including commercial applications, and to alter it and
   redistribute it freely, subject to the following restrictions:
 
   1. The origin of this software must not be misrepresented; you
   must not claim that you wrote the original software. If you use
   this software in a product, an acknowledgment in the product
   documentation would be appreciated but is not required.
 
   2. Altered source versions must be plainly marked as such, and
   must not be misrepresented as being the original software.
 
   3. This notice may not be removed or altered from any source
   distribution.
 
*/
#pragma once
#ifndef __FILE_UTILSOGREDSHOW_PRIVATE_INCLUDED
#define __FILE_UTILSOGREDSHOW_PRIVATE_INCLUDED
 
//#include <Qedit.h>// for sample grabber
#include <dshow.h>
#include <WTypes.h>
// <hack>
// Vite Falcon: Nasty hack to avoid the missing 'qedit.h'
#include <windows.h>
 
EXTERN_C const CLSID CLSID_SampleGrabber;
class DECLSPEC_UUID("C1F400A0-3F08-11d3-9F0B-006008039E37")
	SampleGrabber;
 
EXTERN_C const CLSID CLSID_NullRenderer;
class DECLSPEC_UUID("C1F400A4-3F08-11d3-9F0B-006008039E37")
	NullRenderer;
 
EXTERN_C const IID IID_ISampleGrabberCB;
MIDL_INTERFACE("0579154A-2B53-4994-B0D0-E773148EFF85")
ISampleGrabberCB : public IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE SampleCB( 
		double SampleTime,
		IMediaSample *pSample) = 0;
 
	virtual HRESULT STDMETHODCALLTYPE BufferCB( 
		double SampleTime,
		BYTE *pBuffer,
		long BufferLen) = 0;
 
};
 
EXTERN_C const IID IID_ISampleGrabber;
MIDL_INTERFACE("6B652FFF-11FE-4fce-92AD-0266B5D7C78F")
ISampleGrabber : public IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE SetOneShot( 
		BOOL OneShot) = 0;
 
	virtual HRESULT STDMETHODCALLTYPE SetMediaType( 
		const AM_MEDIA_TYPE *pType) = 0;
 
	virtual HRESULT STDMETHODCALLTYPE GetConnectedMediaType( 
		AM_MEDIA_TYPE *pType) = 0;
 
	virtual HRESULT STDMETHODCALLTYPE SetBufferSamples( 
		BOOL BufferThem) = 0;
 
	virtual HRESULT STDMETHODCALLTYPE GetCurrentBuffer( 
		/* [out][in] */ long *pBufferSize,
		/* [out] */ long *pBuffer) = 0;
 
	virtual HRESULT STDMETHODCALLTYPE GetCurrentSample( 
		/* [retval][out] */ IMediaSample **ppSample) = 0;
 
	virtual HRESULT STDMETHODCALLTYPE SetCallback( 
		ISampleGrabberCB *pCallback,
		long WhichMethodToCallback) = 0;
 
};
// </hack>
 
namespace OgreUtils
{
    struct DirectShowData
    {
        /// Graph object
        IGraphBuilder *pGraph;
        /// Media control object
        IMediaControl *pControl;
        /// Media event object
        IMediaEvent *pEvent;
        /// Grabber filter
        IBaseFilter *pGrabberF;
        /// Grabber object
        ISampleGrabber *pGrabber;
        /// Interface for seeking object
        IMediaSeeking *pSeeking;
        /// Window interface
        /** Useful for some configuration
        */
        IVideoWindow *pWindow;
 
        /// Video output width
        int videoWidth;
        /// Video output height
        int videoHeight;
    };
 
    /// Util function for converting C strings to wide strings
    /** (as needed for path in directshow). */
    WCHAR* util_convertCStringToWString(const char* string);
}
 
#endif // __FILE_UTILSOGREDSHOW_PRIVATE_INCLUDED