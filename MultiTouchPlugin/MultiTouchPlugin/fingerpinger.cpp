// ==============================================================================
//	fingerpinger.cpp
//	
//	tracking fingers on a macbook trackpad
//	
//	Authors:	Michael & Max Egger
//	Copyright:	2009 [ a n y m a ]
//	Website:	www.anyma.ch
//
//  Based on: 	http://www.steike.com/code/multitouch/
//	
//	License:	GNU GPL 2.0 www.gnu.org
//	
//	Version:	2009-06-01
// ==============================================================================



#include "ext.h"  				// you must include this - it contains the external object's link to available Max functions
#include "ext_common.h"
#include <unistd.h>
#include <assert.h>
#include <vector>
#include <algorithm>
#include <CoreFoundation/CoreFoundation.h>
#include "MultitouchSupport.h"

// ==============================================================================
// Our External's Memory structure
// ------------------------------------------------------------------------------

typedef struct _multitouch				// defines our object's internal variables for each instance in a patch
{
	t_object 		p_ob;				// object header - ALL max external MUST begin with this...
	void 			*m_out_list;		// outlet for finger data		
	void			*m_out_bang;		// bang out for start of frame
} t_multitouch;

static void *multitouch_class;			// global pointer to the object class - so max can reference the object 

static std::vector<t_multitouch*> instances;		// we can't pass a reference to our object to the callback function
													// so we keep a list of active instances that we can loop through in the callback function
													
static MTDeviceRef dev;								// reference to the trackpad

// ==============================================================================
// Function Prototypes
// ------------------------------------------------------------------------------

extern "C" {int main(void);}

static void *multitouch_new(t_symbol *s);
static void multitouch_free(t_multitouch *x);
static void multitouch_int(t_multitouch *x,long n);				


// ==============================================================================
// Implementation
// ------------------------------------------------------------------------------


//--------------------------------------------------------------------------
// - Callback
//--------------------------------------------------------------------------

static int callback(int device, Finger *data, int nFingers, double timestamp, int frame) {
	int i;
	t_atom				myList[12];

	for (std::vector<t_multitouch*>::const_iterator it = instances.begin(); it != instances.end(); ++it)
		outlet_bang((*it)->m_out_bang);

	for (i=0; i < nFingers; i++) {
		Finger *f = &data[i];
		SETFLOAT(myList,i);
		SETFLOAT(myList + 1 , f->frame);
		SETFLOAT(myList + 2 , f->angle);
		SETFLOAT(myList + 3 , f->majorAxis);
		SETFLOAT(myList + 4 , f->minorAxis);
		SETFLOAT(myList + 5 , f->normalized.pos.x);
		SETFLOAT(myList + 6 , f->normalized.pos.y);
		SETFLOAT(myList + 7 , f->normalized.vel.x);
		SETFLOAT(myList + 8 , f->normalized.vel.y);
		SETFLOAT(myList + 9 , f->identifier);
		SETFLOAT(myList + 10 , f->state);
		SETFLOAT(myList + 11 , f->size);
		for (std::vector<t_multitouch*>::const_iterator it = instances.begin(); it != instances.end(); ++it)
			outlet_list((*it)->m_out_list, 0L,12,myList);

  }
  return 0;
}

//--------------------------------------------------------------------------
// - On / Off
//--------------------------------------------------------------------------

static void multitouch_int(t_multitouch *x, long n) {
	if (n) {
		if (instances.empty()) {
			dev = MTDeviceCreateDefault();
		  	MTRegisterContactFrameCallback(dev, callback);
		  	MTDeviceStart(dev);
		}
		if (find(instances.begin(), instances.end(), x) == instances.end())
			instances.push_back(x);
	} else {
		std::vector<t_multitouch*>::iterator it = find(instances.begin(), instances.end(), x);
		if (it != instances.end())
			instances.erase(it);
		assert(find(instances.begin(), instances.end(), x) == instances.end());
		if (instances.empty()) {
			MTDeviceStop(dev);
			MTDeviceRelease(dev);
			dev = NULL;
		}
	}
}



//--------------------------------------------------------------------------
// - Object creation and setup
//--------------------------------------------------------------------------

int main(void)
{
	setup((t_messlist **)&multitouch_class, (method)multitouch_new, (method)multitouch_free, (short)sizeof(t_multitouch), 0L, A_DEFSYM, 0); 

	addint((method)multitouch_int);
		
	return 1;
}

//--------------------------------------------------------------------------
// - Object creation
//--------------------------------------------------------------------------

static void *multitouch_new(t_symbol *s)		
{
	t_multitouch *x;											// local variable (pointer to a t_multitouch data structure)

	x = (t_multitouch *)newobject(multitouch_class); 			// create a new instance of this object
	x->m_out_bang = listout(x);
	x->m_out_list = listout(x);

	return x;													// return a reference to the object instance 
}



//--------------------------------------------------------------------------
// - Object destruction
//--------------------------------------------------------------------------

static void multitouch_free(t_multitouch *x)
{
	multitouch_int(x, 0);										// make sure callback is released before deleting the object
}




