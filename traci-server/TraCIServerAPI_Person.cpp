/****************************************************************************/
/// @file    TraCIServerAPI_Person.cpp
/// @author  Daniel Krajzewicz
/// @author  Laura Bieker
/// @author  Christoph Sommer
/// @author  Michael Behrisch
/// @author  Bjoern Hendriks
/// @author  Nicole Ronald
/// @date    05.11.2013
/// @version $Id: TraCIServerAPI_Vehicle.cpp 14494 2013-08-24 21:47:48Z behrisch $
///
// APIs for getting/setting person values via TraCI
/****************************************************************************/
// SUMO, Simulation of Urban MObility; see http://sumo-sim.org/
// Copyright (C) 2001-2013 DLR (http://www.dlr.de/) and contributors
/****************************************************************************/
//
//   This file is part of SUMO.
//   SUMO is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.
//
/****************************************************************************/


// ===========================================================================
// included modules
// ===========================================================================
#ifdef _MSC_VER
#include <windows_config.h>
#else
#include <config.h>
#endif

#ifndef NO_TRACI

#include <microsim/MSNet.h>
#include <microsim/MSInsertionControl.h>
#include <microsim/MSPerson.h>
#include <microsim/MSPersonControl.h>
#include <microsim/MSLane.h>
#include <microsim/MSEdge.h>
#include <microsim/MSEdgeWeightsStorage.h>
#include <utils/geom/PositionVector.h>
#include "TraCIConstants.h"
#include "TraCIServerAPI_Simulation.h"
#include "TraCIServerAPI_Person.h"

#ifdef CHECK_MEMORY_LEAKS
#include <foreign/nvwa/debug_new.h>
#endif // CHECK_MEMORY_LEAKS


// ===========================================================================
// method definitions
// ===========================================================================
bool
TraCIServerAPI_Person::processGet(TraCIServer& server, tcpip::Storage& inputStorage,
                                   tcpip::Storage& outputStorage) {
    // variable & id
    int variable = inputStorage.readUnsignedByte();
    std::string id = inputStorage.readString();
    // check variable
    if (variable != ID_LIST && variable != ID_COUNT && variable != VAR_STAGE && variable != VAR_ROAD_ID
       ) {
        return server.writeErrorStatusCmd(CMD_GET_PERSON_VARIABLE, "Get Person Variable: unsupported variable specified", outputStorage);
    }
    // begin response building
    tcpip::Storage tempMsg;
    //  response-code, variableID, objectID
    tempMsg.writeUnsignedByte(RESPONSE_GET_PERSON_VARIABLE);
    tempMsg.writeUnsignedByte(variable);
    tempMsg.writeString(id);
    // process request
    if (variable == ID_LIST || variable == ID_COUNT) {
        std::vector<std::string> ids;
        MSPersonControl& c = MSNet::getInstance()->getPersonControl();
        for (MSPersonControl::constPersIt i = c.persBegin(); i != c.persEnd(); ++i) {
                ids.push_back((*i).first);
        }
        if (variable == ID_LIST) {
            tempMsg.writeUnsignedByte(TYPE_STRINGLIST);
            tempMsg.writeStringList(ids);
        } else {
            tempMsg.writeUnsignedByte(TYPE_INTEGER);
            tempMsg.writeInt((int) ids.size());
        }
    } else {
		MSPerson* sumoPerson = MSNet::getInstance()->getPersonControl().getPerson(id);
        if (sumoPerson == 0) {
            return server.writeErrorStatusCmd(CMD_GET_PERSON_VARIABLE, "Person '" + id + "' is not known", outputStorage);
        }
        
        switch (variable) {
            
		case VAR_STAGE:
                tempMsg.writeUnsignedByte(TYPE_STRING);
                tempMsg.writeString(sumoPerson->getCurrentStageTypeName());
                break;
		case VAR_ROAD_ID:
                tempMsg.writeUnsignedByte(TYPE_STRING);
				tempMsg.writeString((sumoPerson->getEdge())->getID());
				break;
		}
            
           
    }
    server.writeStatusCmd(CMD_GET_PERSON_VARIABLE, RTYPE_OK, "", outputStorage);
    server.writeResponseWithLength(outputStorage, tempMsg);
    return true;
}


bool
TraCIServerAPI_Person::processSet(TraCIServer& server, tcpip::Storage& inputStorage,
                                   tcpip::Storage& outputStorage) {

	return false;
}

#endif


/****************************************************************************/

