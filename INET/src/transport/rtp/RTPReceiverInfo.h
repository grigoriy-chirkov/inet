/***************************************************************************
                       RTPReceiverInfo.h  -  description
                             -------------------
    (C) 2007 Ahmed Ayadi  <ahmed.ayadi@sophia.inria.fr>
    (C) 2001 Matthias Oppitz <Matthias.Oppitz@gmx.de>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


/** \file RTPReceiverInfo.h
 * This file declares the class RTPReceiverInfo.
 */

#ifndef __RTPRECEIVERINFO_H__
#define __RTPRECEIVERINFO_H__

#include <cassert>
#include "INETDefs.h"
#include "RTPParticipantInfo.h"

/**
 * This class, a subclass of RTPParticipantInfo, is used for storing information
 * about other rtp endsystems.
 * This class processes rtp packets, rtcp sender reports and rtcp sdes chunks
 * originating from this endsystem.
 */
class INET_API RTPReceiverInfo : public RTPParticipantInfo
{

    public:

        /**
         * Default constructor.
         */
        RTPReceiverInfo(uint32_t ssrc = 0);

        /**
         * Copy constructor.
         */
        RTPReceiverInfo(const RTPReceiverInfo& receiverInfo);

        /**
         * Destructor.
         */
        virtual ~RTPReceiverInfo();

        /**
         * Assignment operator.
         */
        RTPReceiverInfo& operator=(const RTPReceiverInfo& receiverInfo);

        /**
         * Duplicates this RTPReceiverInfo by calling the copy constructor.
         */
        virtual RTPReceiverInfo *dup() const;

        /**
         * Extracts information of the given RTPPacket.
         * Also sets _inactiveIntervals to 0.
         */
        virtual void processRTPPacket(RTPPacket *packet, int id, simtime_t arrivalTime);

        /**
         * Extracts information of the given SenderReport.
         */
        virtual void processSenderReport(SenderReport *report, simtime_t arrivalTime);

        /**
         * Extracts information of the given SDESChunk.
         */
        virtual void processSDESChunk(SDESChunk *sdesChunk, simtime_t arrivalTime);

        /**
         * Returns a ReceptionReport if this rtp end system is a sender,
         * NULL otherwise.
         */
        virtual ReceptionReport *receptionReport(simtime_t now);

        /**
         * Informs this RTPReceiverInfo that one rtcp interval has past.
         */
        virtual void nextInterval(simtime_t now);

        /**
         * Returns true if this rtp end system is regarded active.
         */
        virtual bool active();

        /**
         * Returns true if this rtp end system is regarded valid.
         */
        virtual bool valid();

        /**
         * Returns true if this rtp end system should be deleted from
         * the list of known rtp session participant.
         * This method should be called directly after nextInterval().
         */
        virtual bool toBeDeleted(simtime_t now);


    private:

        /**
         * The sequence number of the first RTPPacket received.
         */
        uint16_t _sequenceNumberBase;

        /**
         * The highest sequence number of an RTPPacket received.
         */
        uint16_t _highestSequenceNumber;

        /**
         * The highest sequence number of an RTPPacket received
         * before the beginning of the current rtcp interval.
         */
        uint32_t _highestSequenceNumberPrior;

        /**
         * The number of sequence number wrap arounds.
         */
        uint32_t _sequenceNumberCycles;

        /**
         * How many rtp packets from this source have been received.
         */
        uint32_t _packetsReceived;

        /**
         * How many rtp packets have been received from this source
         * before the current rtcp interval began.
         */
        uint32_t _packetsReceivedPrior;

        /**
         * The interarrival jitter. See rtp rfc for details.
         */
        simtime_t _jitter;

        /**
         * The output vector for jitter value
         */
        cOutVector _jitterOutVector;
        /**
         * The output vector for packet lost
         */
        cOutVector _packetLostOutVector;
        /**
         * The clock rate (in ticks per second) the sender increases the
         * rtp timestamps. It is calculated when two sender reports have
         * been received.
        */
        int _clockRate;

        /**
         * The rtp time stamp of the last SenderReport received from this sender.
         */
        uint32_t _lastSenderReportRTPTimeStamp;

        /**
         * The ntp time stamp of the last SenderReport received from this sender.
         */
        uint64_t _lastSenderReportNTPTimeStamp;

        /**
         * The rtp time stamp of the last RTPPacket received from this sender.
         * Needed for calculating the jitter.
         */
        uint32_t _lastPacketRTPTimeStamp;

        /**
         * The arrival time of the last RTPPacket received from this sender.
         * Needed for calculating the jitter.
         */
        simtime_t _lastPacketArrivalTime;

        /**
         * The arrival time of the last SenderReport received from this sender.
         */
        simtime_t _lastSenderReportArrivalTime;

        /**
         * The consecutive number of rtcp intervals this rtcp end system
         * hasn't sent anything.
         */
        int _inactiveIntervals;

        /**
         * The time when this rtp end system has been inactive for five
         * consecutive rtcp intervals.
         */
        simtime_t _startOfInactivity;

        /**
         * The number of rtp and rtcp packets received from this rtp end system.
         */
        int _itemsReceived;

        int packetLoss;

        FILE * packetSequenceLostLogFile ;
};

#endif

