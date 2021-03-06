#include <CoreFoundation/CoreFoundation.h>
#include <CoreServices/CoreServices.h>
#include <QuickLook/QuickLook.h>
#include "MyQuickLook.h"
#include <zlib.h>
#import <Foundation/Foundation.h>
#include "nopic.h"

/* -----------------------------------------------------------------------------
   Generate a preview for file

   This function's job is to create preview for designated file
   ----------------------------------------------------------------------------- */

static OSStatus GeneratePreviewForFreeze(void *thisInterface, QLPreviewRequestRef preview, NSURL *url, NSDictionary *options);
static OSStatus GeneratePreviewForMemCard(void *thisInterface, QLPreviewRequestRef preview, NSURL *url, NSDictionary *options);

OSStatus GeneratePreviewForURL(void *thisInterface, QLPreviewRequestRef preview, CFURLRef url, CFStringRef contentTypeUTI, CFDictionaryRef options)
{
	OSStatus theStatus = noErr;
	@autoreleasepool {
		NSWorkspace *workspace = [NSWorkspace sharedWorkspace];
		NSURL *urlNS = (__bridge NSURL*)url;
		NSString *uti = (__bridge NSString*)contentTypeUTI;
		NSDictionary *optionsNS = (__bridge NSDictionary*)options;
		if ([workspace type:uti conformsToType:@"com.codeplex.pcsxr.freeze"]) {
			theStatus = GeneratePreviewForFreeze(thisInterface, preview, urlNS, optionsNS);
		} else if ([workspace type:uti conformsToType:@"com.codeplex.pcsxr.memcard"]) {
			theStatus = GeneratePreviewForMemCard(thisInterface, preview, urlNS, optionsNS);
		}
	}
	return theStatus;
}

void CancelPreviewGeneration(void *thisInterface, QLPreviewRequestRef preview)
{
	// Implement only if supported
}

OSStatus GeneratePreviewForFreeze(void *thisInterface, QLPreviewRequestRef preview, NSURL *url, NSDictionary *options)
{
	NSData *data;
	gzFile f;
	const char* state_filename;
	if ([url respondsToSelector:@selector(fileSystemRepresentation)]) {
		state_filename = [url fileSystemRepresentation];
	} else {
		state_filename = [[url path] fileSystemRepresentation];
	}
	
	if (!state_filename) {
		return fnfErr;
	}
	
	unsigned char *pMem = (unsigned char *) malloc(128*96*3);
	if (pMem == NULL)
		return mFulErr;
	
	f = gzopen(state_filename, "rb");
	if (f != NULL) {
		gzseek(f, 32, SEEK_SET); // skip header
		gzseek(f, sizeof(uint32_t), SEEK_CUR);
		gzseek(f, sizeof(uint8_t), SEEK_CUR);
		gzread(f, pMem, 128*96*3);
		gzclose(f);
	} else {
		memcpy(pMem, NoPic_Image.pixel_data, 128*96*3);
	}
	
	NSBitmapImageRep *imRep = [[NSBitmapImageRep alloc] initWithBitmapDataPlanes:&pMem pixelsWide:NoPic_Image.width pixelsHigh:NoPic_Image.height bitsPerSample:8 samplesPerPixel:3 hasAlpha:NO isPlanar:NO colorSpaceName:NSCalibratedRGBColorSpace bitmapFormat:0 bytesPerRow:NoPic_Image.width * NoPic_Image.bytes_per_pixel bitsPerPixel:24];
	if (imRep) {
		data = [imRep TIFFRepresentation];
		QLPreviewRequestSetDataRepresentation(preview, (__bridge CFDataRef)(data), kUTTypeImage, NULL);
	}
	free(pMem);
	return noErr;
}

static OSStatus GeneratePreviewForMemCard(void *thisInterface, QLPreviewRequestRef preview, NSURL *url, NSDictionary *options)
{
	return unimpErr;
}
