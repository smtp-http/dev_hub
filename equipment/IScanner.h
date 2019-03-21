#ifndef __I_SCANNER_H__
#define __I_SCANNER_H__

#include <string>
#include <sys/time.h>


using namespace std;


typedef struct {
	string Barcode;
	unsigned int ScannerId;
	long int CurTime;
}ScannerBarcode;

class IRecvBarcode {
public:
	virtual int OnBarcode(ScannerBarcode *code) = 0;
};


#endif
