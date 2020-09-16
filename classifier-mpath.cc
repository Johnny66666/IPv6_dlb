/* -*-	Mode:C++; c-basic-offset:8; tab-width:8; indent-tabs-mode:t -*- */

/*
 * Copyright (C) 1997 by the University of Southern California
 * $Id: classifier-mpath.cc,v 1.10 2005/08/25 18:58:01 johnh Exp $
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.
 *
 *
 * The copyright of this module includes the following
 * linking-with-specific-other-licenses addition:
 *
 * In addition, as a special exception, the copyright holders of
 * this module give you permission to combine (via static or
 * dynamic linking) this module with free software programs or
 * libraries that are released under the GNU LGPL and with code
 * included in the standard release of ns-2 under the Apache 2.0
 * license or under otherwise-compatible licenses with advertising
 * requirements (or modified versions of such code, with unchanged
 * license).  You may copy and distribute such a system following the
 * terms of the GNU GPL for this module and the licenses of the
 * other code concerned, provided that you include the source code of
 * that other code when and as the GNU GPL requires distribution of
 * source code.
 *
 * Note that people who make modified versions of this module
 * are not obligated to grant this special exception for their
 * modified versions; it is their choice whether to do so.  The GNU
 * General Public License gives permission to release a modified
 * version without this exception; this exception also makes it
 * possible to release a modified version which carries forward this
 * exception.
 *
 */
/*
*function:DRILL (SIGCOMM2017)
*author:ljl
*time:20190122
*/
#ifndef lint
static const char rcsid[] =
    "@(#) $Header: /cvsroot/nsnam/ns-2/classifier/classifier-mpath.cc,v 1.10 2005/08/25 18:58:01 johnh Exp $ (USC/ISI)";
#endif

#include "classifier.h"
#include<stdio.h> 
#include<sys/time.h>
#include<time.h>
#include "random.h" 
#include "ip.h"
#include "queue.h"
#include "packet.h"
#include "math.h"
#include "tcp.h"

int pathnum=0;  //the number of path
int last_id1=0; //the last selected path id
int last_load1=0; //the last selected path load
int last_id2=0; //the last selected path id
int last_load2=0; //the last selected path load
int last_id3=0; //the last selected path id
int last_load3=0; //the last selected path load
int last_id4=0; //the last selected path id
int last_load4=0; //the last selected path load  

//--------------------------------------------------------------select path---------------------------------------------------
//DRILL:compare queue lengths of two random ports plus the previously least-loaded port, send the packet to the least loaded of these.
int ptc1[2]={0,0};  //ptc[0] is the selected queue length, ptc[1] is the selected queue id.
//int temp[2] = {0,0};
/*
int *DRILL1(int a[],int c,int l_id, int l_load){       //a[] is the length of queue,c is the number of paths,l_id is thelast selected path id. l_load is the last selected path load 
    int i =(int)Random::uniform(1,c+1);
    int j =(int)Random::uniform(1,c+1);
    if(a[i]<=a[j]){
       if(a[i]<=l_load){
           ptc1[0]=a[i];
           ptc1[1]=i;
       }else{
           ptc1[0]=a[l_id];
           ptc1[1]=l_id;
       }      
    }else{
       if(a[j]<=l_load){
           ptc1[0]=a[j];
           ptc1[1]=j;
       }else{
           ptc1[0]=a[l_id];
           ptc1[1]=l_id;
       }   
    }
    last_id1=ptc1[1];
    last_load1=ptc1[0];
    return ptc1;
}
*/
int temp[2] = {0,0};
int best[2] = {0,0};
int result[2] = {0,0};
int flow[100] = {0};
int *dlb(int a[]){
	//int temp[2] = {0,0};
temp[0]=a[1];
temp[1]=1;
best[0]=a[1];
best[1]=1;
result[0] = a[0];
result[1] = 1;
	for(int i=2;i<=3;i++){
		if (a[i] < temp[0]){
			temp[0] = a[i];
			temp[1] = i;
			}
		if (a[i] > best[0]){
			best[0] = a[i];
			best[1] = i;
			}
		}
	if((best[0] - temp[0]) > 3 ){
		result[0] = temp[0];
		result[1] = temp[1];
		}
	else{
		result[0] = -1;
		result[1] = -1;
		}	
	return result;
}



class MultiPathForwarder : public Classifier {
public:
	MultiPathForwarder() : ns_(0) {} 
	virtual int classify(Packet* p) {
		     int cl=0;
		     int fail = ns_; 
		     hdr_ip* iph=hdr_ip::access(p);
		     
		     hdr_tcp *tcph = hdr_tcp::access(p);
		     int seq = tcph->seqno();
                     int fid = iph->flowid();
		     int databytes=hdr_cmn::access(p)->size();
                     
                     pathnum=2;
                     //if(fid<100){
                     //if(fid<25){
			if(Scheduler::instance().clock() > 0.19){
				printf("len %d fid %d\n",qlenAr[1],fid);

				}
                          do {
                                   //cl =DRILL1(qlenAr,pathnum,last_id1,last_load1)[1]-1;

				   //cl =dlb(qlenAr)[1]-1;


				   if(seq == 1 && databytes > 500){
					cl = (int)Random::uniform(0,maxslot_+1);
					flow[fid] = cl;
					}
				   else{
					cl = flow[fid];
					}
				   if(dlb(qlenAr)[1] >= 0 ){
					cl =dlb(qlenAr)[1]-1;
					flow[fid] = cl;
					}


                        
	       	          } while (slot_[cl] == 0 && ns_ != fail);
                     //}
                    
//Tcl::insance().evalf("puts [format \"time: %f cl: %d maxslot: %d q1: %d %d %d %d %d %d %d %d %d %d\"]",Scheduler::instance().clock(),cl+1,maxslot_+1,qlenAr[1],qlenAr[2],qlenAr[3],qlenAr[4],qlenAr[5],qlenAr[6],qlenAr[7],qlenAr[8],qlenAr[9],qlenAr[10]);
                  
		    return cl;

	}
private:
	int ns_;
};

static class MultiPathClass : public TclClass {
public:
	MultiPathClass() : TclClass("Classifier/MultiPath") {} 
	TclObject* create(int, const char*const*) {
		return (new MultiPathForwarder());
	}
} class_multipath;
