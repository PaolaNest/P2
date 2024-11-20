#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "pav_analysis.h"

#include "vad.h"

const float FRAME_TIME = 10.0F; /* Frames de 10 ms */
const float MAX_DURATION = FRAME_TIME * 3.0F; /* Maximum duration in ms for an undefined state */
// zcr_threshold_voice = 800;
//zcr_threshold_silence =795;

/* 
 * As the output state is only ST_VOICE, ST_SILENCE, or ST_UNDEF,
 * only this labels are needed. You need to add all labels, in case
 * you want to print the internal state in string format
 */

const char *state_str[] = {
  "UNDEF", "S", "V", "INIT"
};

const char *state2str(VAD_STATE st) {
  return state_str[st];
}

/* Define a datatype with interesting features */
typedef struct {
  float zcr;
  float p;
  float am;
} Features;

/* 
 * TODO: Delete and use your own features!
 */

// Features compute_features(const float *x, int N) {
//   /*
//    * Input: x[i] : i=0 .... N-1 
//    * Ouput: computed features
//    */
//   /* 
//    * DELETE and include a call to your own functions
//    *
//    * For the moment, compute random value between 0 and 1 
//    */
//   Features feat;
//   feat.p = compute_power(x,N);
//   feat.zcr = compute_zcr(x,N,16000);
//   feat.am = compute_am(x,N);
//   return feat;
// }

Features compute_features(const float *x, int N) {
    Features feat;
    feat.p = compute_power(x, N);          // Potencia
    feat.zcr = compute_zcr(x, N, 16000);   // Cruces por cero
    feat.am = compute_am(x, N);            // Amplitud media
    return feat;
}

/* 
 * TODO: Init the values of vad_data
 */

VAD_DATA * vad_open(float rate) {
  VAD_DATA *vad_data = malloc(sizeof(VAD_DATA));
  vad_data->state = ST_INIT;
  vad_data->sampling_rate = rate;
  vad_data->frame_length = rate * FRAME_TIME * 1e-3;

  return vad_data;
}

VAD_STATE vad_close(VAD_DATA *vad_data) {
  VAD_STATE state = vad_data->state;
  if (state == ST_UNDEF) {
    state = ST_SILENCE; //Els declarem com silenci
  }

  free(vad_data);
  return state;
}

unsigned int vad_frame_size(VAD_DATA *vad_data) {
  return vad_data->frame_length;
}

/* 
 * TODO: Implement the Voice Activity Detection 
 * using a Finite State Automata
 */

VAD_STATE vad(VAD_DATA *vad_data, float *x, float alfa1, float alfa2, float MAX_num_trames) {

  /* 
   * TODO: You can change this, using your own features,
   * program finite state automaton, define conditions, etc.
   */

  Features f = compute_features(x, vad_data->frame_length);
  vad_data->last_feature = f.p; /* save feature, in case you want to show */
  vad_data->last_feature_zcr = f.zcr;

  switch (vad_data->state) {
    case ST_INIT:
      vad_data->state = ST_SILENCE;
      vad_data->p1 = f.p;
      break;

    case ST_SILENCE:
        //&& f.zcr > zcr_threshold_voice
      if (f.p > vad_data->p1 + alfa1 ) {
        vad_data->state = ST_UNDEF;
        vad_data->undef_count = 0;
      }
      break;

    case ST_VOICE:
      if (f.p < vad_data->p1 + alfa2) {
        vad_data->state = ST_UNDEF;
        vad_data->undef_count = 0;
      }
      break;

    case ST_UNDEF:
    //Canvi de silenci a veu
       // && f.zcr > zcr_threshold_voice
      if (f.p > vad_data->p1 + alfa1 ) { 
        // Si supera el llindar de silenci 
            
        if (vad_data->undef_count < MAX_num_trames +1){
          vad_data->undef_count = vad_data->undef_count +1;
          vad_data->state = ST_UNDEF;
        }  
        else
          vad_data->state = ST_VOICE;
      } 
      else {
          vad_data->state = ST_SILENCE;         
      } 

    //Canvi de veu a silenci
       // && f.zcr < zcr_threshold_silence
      if (f.p < vad_data->p1 + alfa2 ) {
        // Si no supera el llindar de veu
        if (vad_data->undef_count < MAX_num_trames+1){
          vad_data->undef_count++;
          vad_data->state = ST_UNDEF;
        }  
        else
          vad_data->state = ST_SILENCE;
      } 
      else {
           vad_data->state = ST_VOICE;
      } 

      break;
  }

  //if (vad_data->state == ST_SILENCE || vad_data->state == ST_VOICE) {
    return vad_data->state;
  //}
  //else return ST_UNDEF;
}


void vad_show_state(const VAD_DATA *vad_data, FILE *out) {
  fprintf(out, "%d\t%f\n", vad_data->state, vad_data->last_feature);
}
