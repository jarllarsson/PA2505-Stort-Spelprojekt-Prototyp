#ifndef LowPass_h__
#define LowPass_h__
#include <XAPOBase.h>
#include <assert.h>


class LowPass : public CXAPOBase
{
private:
	int uChannels;
	int uBytesPerSample;
public:
	LowPass(const XAPO_REGISTRATION_PROPERTIES* pRegProperties):CXAPOBase(pRegProperties){};
	~LowPass(){};
	STDMETHOD(LockForProcess) (
		UINT32 InputLockedParameterCount,
		const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS  *pInputLockedParameters,
		UINT32 OutputLockedParameterCount,
		const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS  *pOutputLockedParameters
		){
			assert(!IsLocked());
			assert(InputLockedParameterCount == 1);
			assert(OutputLockedParameterCount == 1);
			assert(pInputLockedParameters != NULL);
			assert(pOutputLockedParameters != NULL);
			assert(pInputLockedParameters[0].pFormat != NULL);
			assert(pOutputLockedParameters[0].pFormat != NULL);


			uChannels = pInputLockedParameters[0].pFormat->nChannels;
			uBytesPerSample = (pInputLockedParameters[0].pFormat->wBitsPerSample >> 3);

			return CXAPOBase::LockForProcess(
				InputLockedParameterCount,
				pInputLockedParameters,
				OutputLockedParameterCount,
				pOutputLockedParameters);
	}
	STDMETHOD_(void, Process)(
		UINT32 InputProcessParameterCount,
		const XAPO_PROCESS_BUFFER_PARAMETERS *pInputProcessParameters,
		UINT32 OutputProcessParameterCount,
		XAPO_PROCESS_BUFFER_PARAMETERS *pOutputProcessParameters,
		BOOL IsEnabled
		){
			assert(IsLocked());
			assert(InputProcessParameterCount == 1);
			assert(OutputProcessParameterCount == 1);
			assert(NULL != pInputProcessParameters);
			assert(NULL != pOutputProcessParameters);


			XAPO_BUFFER_FLAGS inFlags = pInputProcessParameters[0].BufferFlags;
			XAPO_BUFFER_FLAGS outFlags = pOutputProcessParameters[0].BufferFlags;

			// assert buffer flags are legitimate
			assert(inFlags == XAPO_BUFFER_VALID || inFlags == XAPO_BUFFER_SILENT);
			assert(outFlags == XAPO_BUFFER_VALID || outFlags == XAPO_BUFFER_SILENT);

			// check input APO_BUFFER_FLAGS
			switch (inFlags)
			{
			case XAPO_BUFFER_VALID:
				{
					void* pvSrc = pInputProcessParameters[0].pBuffer;
					assert(pvSrc != NULL);

					void* pvDst = pOutputProcessParameters[0].pBuffer;
					assert(pvDst != NULL);

					memcpy(pvDst,pvSrc,pInputProcessParameters[0].ValidFrameCount * uChannels * uBytesPerSample);
					break;
				}

			case XAPO_BUFFER_SILENT:
				{
					// All that needs to be done for this case is setting the
					// output buffer flag to XAPO_BUFFER_SILENT which is done below.
					break;
				}

			}

			// set destination valid frame count, and buffer flags
			pOutputProcessParameters[0].ValidFrameCount = pInputProcessParameters[0].ValidFrameCount; // set destination frame count same as source
			pOutputProcessParameters[0].BufferFlags     = pInputProcessParameters[0].BufferFlags;     // set destination buffer flags same as source

	}
};

#endif // LowPass_h__