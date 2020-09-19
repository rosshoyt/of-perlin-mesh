//
//  MidiNotesState.h
//  midiVisualizer
//
//  Created by Ross Hoyt on 7/27/20.
//

#ifndef MidiNotesState_h
#define MidiNotesState_h
#include "ofxMidi.h"

typedef std::unique_lock<std::mutex> Lock;

class MidiCCNode {
public:
    MidiCCNode() : value(), mtx(){}
    
    int getValue(){
        Lock lck(mtx, std::defer_lock);
        lck.lock();
        int cpy = value;
        lck.unlock();
        return cpy;
    }
    
    int setValue(int value){
        Lock lck(mtx, std::defer_lock);
        lck.lock();
        this->value = value;
        lck.unlock();
    }
    
private:
    int value;
    std::mutex mtx;
};



/**
 * Class encapsulating the state of a single MIDI channel. Processes NoteOn, NoteOff, and all Control Change Messages.
 * Supports concurrent read/write access across multiple threads via mutex locks
 */
class MIDIChannelState {
public:
    MIDIChannelState() : activeNoteOns(), sustainedNotes(), midiCCState(128), sustained(false), mtx(), mtxSusNotes() {}
    
    void processMIDIMessage(ofxMidiMessage& message){
        switch(message.status) {
            case MIDI_NOTE_ON:
                tryAddNoteOn(message.pitch, message.velocity);
                // TODO refactor channel to process each midi message independantly
                break;
                
            case MIDI_NOTE_OFF:
                tryAddNoteOff(message.pitch);
                break;
                
            case MIDI_CONTROL_CHANGE:
                processMIDICC(message);
                
                break;
        }
    }
    
    void processMIDICC(ofxMidiMessage& message){
        switch(message.control){
            case 1:
                tryAddMIDICC(message.control, message.value);
                break;
            case 64:
                switch(message.value){
                    case 0:
                        //std::cout<< "MIDI Control Change # " << message.control << " value = " << message.value << '\n';
                        setSustainPedalOff();
                        break;
                        
                    case 127:
                        
                        setSustainPedalOn();
                        break;
                }
        }
    }
    
    void tryAddNoteOn(int note, int velocity){
        Lock lck(mtx,std::defer_lock);
        lck.lock();
        // insert or update new note/velocity pair
        activeNoteOns[note] = velocity;
        lck.unlock();
    }
    
    void tryAddNoteOff(int note){
        if(sustained) {
            // move note from activeNoteOns to sustainedNotes
            // record the note's initial (earliest occuring) NoteOn Velocity
            int noteOnVel = activeNoteOns.at(note); // 'concurrent access is safe' @s`ee http://www.cplusplus.com/reference/map/map/at/
            // add to sustainedNotes
            Lock lck2 (mtxSusNotes,std::defer_lock);
            lck2.lock();
            // insert or update note value
            sustainedNotes[note] = noteOnVel;
            lck2.unlock();
        }
        // remove note from activeNoteOnsMap
        Lock lck (mtx,std::defer_lock);
        lck.lock();
        activeNoteOns.erase(note);
        lck.unlock();
        
    }
    
    void setSustainPedalOn(){
        sustained.store(true);
    }
    
    void setSustainPedalOff(){
        if(sustained){
            
            sustained.store(false);
            // clear all notes from sustainedNotes
            Lock lck (mtxSusNotes,std::defer_lock);
            lck.lock();
            sustainedNotes.clear();
            lck.unlock();
        }
    }
    
    std::map<int,int> getNotes(){
        Lock lck (mtx,std::defer_lock);
        lck.lock();
        // copy note map
        auto ret = activeNoteOns;
        lck.unlock();
        return ret;
    }
    std::map<int,int> getSustainedNotes(){
        Lock lck (mtxSusNotes,std::defer_lock);
        lck.lock();
        // copy sus notes map
        auto ret = sustainedNotes;
        lck.unlock();
        return ret;
    }
    
    std::map<int,int> getAllNotes(){
        auto notes = getNotes();
        auto susNotes = getSustainedNotes();
        notes.insert(susNotes.begin(), susNotes.end());
        return notes;
    }
    
    int getNumNotes(){
        Lock lck (mtx,std::defer_lock);
        lck.lock();
        int n = activeNoteOns.size();
        lck.unlock();
        return n;
    }
    
    void tryAddMIDICC(int midiCC, int value) {
        //std::cout << "Recieved midi CC #1 val = " << value << '\n';
        Lock lck (mtx,std::defer_lock);
        lck.lock();
        midiCCState[midiCC].setValue(value);
        lck.unlock();
    }
    
    int tryGetCCValue(int ccNo){
        Lock lck (mtx,std::defer_lock);
        lck.lock();
        int cpy = midiCCState[ccNo].getValue();
        lck.unlock();
        //std::cout << "returning cc#" << ccNo << " value = " << cpy << '\n';
        return cpy;
    }
    
    
    bool sustainPedalIsDown(){
        return sustained.load();
    }
    
private:
    
    std::map<int,int> activeNoteOns;
    std::map<int,int> sustainedNotes;
    
    std::atomic<bool>  sustained;
    
    std::mutex mtx;
    std::mutex mtxSusNotes;
    
    std::vector<MidiCCNode> midiCCState;
    
    
    
};

#endif /* MidiNotesState_h */
