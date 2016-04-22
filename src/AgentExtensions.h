 /**
 * IBM Confidential
 * OCO Source Materials
 * IBM Monitoring and Diagnostic Tools - Health Center
 * (C) Copyright IBM Corp. 2007, 2015 All Rights Reserved.
 * The source code for this program is not published or otherwise
 * divested of its trade secrets, irrespective of what has
 * been deposited with the U.S. Copyright Office.
 */


#ifndef ibmras_monitoring_monitoring_h
#define ibmras_monitoring_monitoring_h
#include <string>
extern "C"{

#ifndef PLUGIN_API_VERSION
#define PLUGIN_API_VERSION "1.0"
#endif

/* provide a default definition of DECL of the platform does not define one */
#ifndef DECL
#define DECL
#endif

#if defined(_WINDOWS)
  #if defined(EXPORT)
    #define DECL __declspec(dllexport)	/* required for DLLs to export the plugin functions */
  #else
    #define DECL __declspec(dllimport)
  #endif
  #define PLUGIN_API_DECL __declspec(dllexport)	/* required for DLLs to export the plugin functions */
#else
  #define PLUGIN_API_DECL
#endif

/*
 * API definitions for data sources to connect to the monitoring
 * agent.
 */

/* data from a source */
typedef struct monitordata {
	unsigned int provID;				/* provider ID, previously allocated during the source registration */
	unsigned int sourceID;			/* source ID, previously supplied by the source during registration */
	unsigned int size;				/* amount of data being provided */
	const char *data;			/* char array of the data to store */
	bool persistent;            /* persistent data will not be removed from the bucket */
} monitordata;

typedef monitordata* (*PULL_CALLBACK)(void);			/* shortcut definition for the pull source callback */
typedef void (*PULL_CALLBACK_COMPLETE)(monitordata*);	/* callback to indicate when the data source can free / re-use the memory */
typedef char* (*GET_CONFIG)(void);

/* common header for data sources */
typedef struct srcheader {
	unsigned int sourceID;			/* ID assigned by the provider - unique by provider */
	unsigned int capacity;			/* the amount of space in bytes that should be allocated for this source */
	const char *name;			/* null terminated C string */
	const char *description;	/* null terminated C string */
} srcheader;

typedef struct pushsource {
	srcheader header;			/* common source header */
	pushsource *next;			/* next source or null if this is the last one in the list */
} pushsource;

typedef struct pullsource{
	srcheader header;			/* common source header */
	pullsource *next;			/* the next source or null if this is the last one in the list */
	unsigned int pullInterval;		/* time in seconds at which data should be pulled from this source */
	PULL_CALLBACK callback;
	PULL_CALLBACK_COMPLETE complete;
} pullsource;

/* definition for connectors */
typedef void* (*CONNECTOR_FACTORY)(const char* properties);	/* short cut for the function pointer to invoke in the connector library */

/* definition for receivers */
typedef void (*RECEIVE_MESSAGE)(const char* id, unsigned int size, void *data);	/* short cut for the function pointer to invoke in the receiver library */

/*
 * Enumeration levels to set for the logger
 */
enum loggingLevel {
	/* log levels are ranked with debug being the most verbose */
	none=0, warning, info, fine, finest, debug
};

typedef void (*pushData)(monitordata *data);
typedef int (*sendMessage)(const char * sourceId, unsigned int size,void *data);
typedef void (*exposedLogger)(loggingLevel lev, const char * message);
typedef const char * (*agentProperty)(const char * key);
typedef void (*setAgentProp)(const char* key, const char* value);
typedef void (*lifeCycle)();
typedef bool (*loadPropFunc)(const char* filename);
typedef const char* (*getVer)();
typedef void (*setLogLvls)();

typedef struct agentCoreFunctions {
	pushData agentPushData;
	sendMessage agentSendMessage;
	exposedLogger logMessage;
	agentProperty getProperty;
} agentCoreFunctions;

typedef struct loaderCoreFunctions {
	lifeCycle init;
	lifeCycle start;
	lifeCycle stop;
	lifeCycle shutdown;
	exposedLogger logMessage;
	agentProperty getProperty;
	setAgentProp setProperty;
	loadPropFunc loadPropertiesFile;
    getVer getAgentVersion;
    setLogLvls setLogLevels; 

} loaderCoreFunctions;


typedef loaderCoreFunctions* (*LOADER_CORE)();

typedef int (*PLUGIN_INITIALIZE)(const char* properties);
typedef pushsource* (*PUSH_SOURCE_REGISTER)(agentCoreFunctions aCF, unsigned int provID);
typedef void (*PUSH_CALLBACK)(monitordata* data);

}
#endif /* ibmras_monitoring_monitoring_h */



