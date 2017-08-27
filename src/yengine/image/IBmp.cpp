#include "IBmp.h"

long IBmp::getImageInfo(FILE* inputFile, long offset, int nbOfChars) {

	unsigned char *ptrC;
	long value = 0L;
	int i;
	unsigned char dummy = '0';
	ptrC = &dummy;

	// Sets the file position of the stream to the given offset.
	// SEEK_SET : This is the position from where offset is added -> Beginning of file
	fseek(inputFile, offset, SEEK_SET);

	for (i = 1; i <= nbOfChars; i++) {

		fread(ptrC, sizeof(char), 1, inputFile);
		// Calculate value based on adding bytes 
		value = (long)(value + (*ptrC)*(pow((double)256, (i - 1))));

	}

	return(value);

};

void IBmp::printData() {

	if (mData != NULL) {

		for (int i = 0; i < mBmpInfo.height; i++) {

			for (int j = 0; j < mBmpInfo.width; j++) {

				printf("%d\t", mData[j + i * mBmpInfo.width]);

			}

			printf("\n");

		}


	}else{

		printf("No data\n");

	}

};

void IBmp::read(const char *bmpInput) {

	FILE *pBmpInput;
	long fileSize;
	unsigned char *pChar;
	unsigned char someChar;
	unsigned char blueValue, greenValue, redValue;
	int	vectorSize, nColors;
	int	r, c;

	// Initialize pointer 
	someChar = '0';
	pChar = &someChar;

	if (mData != NULL) 
		free(mData);

	//printf("Reading : %s", bmpInput);

	pBmpInput = fopen(bmpInput, "rb");

	// Offset added from SEEK_END = end of file
	fseek(pBmpInput, 0L, SEEK_END);

	// GET BMP INFO
	mBmpInfo.size = getImageInfo(pBmpInput, 14, 4);
	mBmpInfo.width = (int)getImageInfo(pBmpInput, 18, 4);
	mBmpInfo.height = (int)getImageInfo(pBmpInput, 22, 4);
	mBmpInfo.bits = (unsigned short int)getImageInfo(pBmpInput, 28, 2);
	mBmpInfo.ncolours = (unsigned int)getImageInfo(pBmpInput, 46, 4);
		
	mBmpHeader.offsetBits = (unsigned long)getImageInfo(pBmpInput, 10, 4);
	mBmpHeader.size = getImageInfo(pBmpInput, 2, 4);
		
	/*printf("\nWidth: %d\n", mBmpInfo.width);
	printf("Height: %d\n", mBmpInfo.height);
	printf("File size: %ld\n", mBmpHeader.size);
	printf("Bits/pixel: %d\n", mBmpInfo.bits);
	printf("Size: %d\n", mBmpInfo.size);*/
	printf("offsetBits: %d\n", mBmpHeader.offsetBits);
	
	fseek(pBmpInput, mBmpHeader.offsetBits, SEEK_SET);

	mData = (unsigned char*)malloc(sizeof(unsigned char) * mBmpInfo.height * mBmpInfo.width * 3);

	if (mData != NULL) {

		// Each data line in BMP is a multiple of 4 in bytes
		// Search padding

		int padding = 0;

		while (((3 * mBmpInfo.width + padding) % 4) != 0) {

			padding++;

		};

		printf("padding : %d\n", padding);
		
		fread(mData, 1, mBmpInfo.height * mBmpInfo.width * 3, pBmpInput);

		/*for (r = mBmpInfo.height - 1; r >= 0; r--) {

			for (c = 0; c < mBmpInfo.width; c++) {

				//READ FIRST BYTE TO GET BLUE VALUE
				fread(pChar, sizeof(char), 1, pBmpInput);
				blueValue = *pChar;

				//READ NEXT BYTE TO GET GREEN VALUE
				fread(pChar, sizeof(char), 1, pBmpInput);
				greenValue = *pChar;

				//READ NEXT BYTE TO GET RED VALUE
				fread(pChar, sizeof(char), 1, pBmpInput);
				redValue = *pChar;

				//unsigned char grayValue = 0.299 * redValue + 0.587 * greenValue + 0.114 * blueValue;
				mData[(c + r * mBmpInfo.width)*3] = redValue;//0.299 * redValue + 0.587 * greenValue + 0.114 * blueValue;
				//printf("(%d;%d) -> %d : %d \n", c, r, c + r * mBmpInfo.width, redValue);

			}

			for (int i = 0; i < padding; i++)
				fread(pChar, sizeof(char), 1, pBmpInput);

		}*/
	
		fclose(pBmpInput);
		
	}else {

		printf("mData NULL\n");

	}
	
};

int IBmp::getWidth() {

	return mBmpInfo.width;

};

int IBmp::getHeight() {

	return mBmpInfo.height;

};

unsigned char * IBmp::getData() {

	return mData;

};