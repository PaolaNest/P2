#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "pav_analysis.h"

#include "vad.h"

const float FRAME_TIME = 10.0F; /* Frames de 10 ms */
const float MAX_DURATION = FRAME_TIME * 3.0F; /* Maximum duration in ms for an undefined state */


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

Features compute_features(const float *x, int N) {
  /*
   * Input: x[i] : i=0 .... N-1 
   * Ouput: computed features
   */
  /* 
   * DELETE and include a call to your own functions
   *
   * For the moment, compute random value between 0 and 1 
   */
  Features feat;
  feat.p = compute_power(x,N);
  feat.zcr = compute_zcr(x,N,16000);
  feat.am = compute_am(x,N);
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

  switch (vad_data->state) {
    //Inicialitzem vad amb l'estat de silenci i guardem la potencia P1 (referència de silenci)
    case ST_INIT:
      vad_data->state = ST_SILENCE;
      vad_data->p1 = f.p;
<<<<<<< HEAD
=======
      vad_data->last_state_known = ST_SILENCE;
<<<<<<< HEAD
    break;

    case ST_SILENCE:
      if (f.p > vad_data->p1 +alfa1) {
        vad_data->state = ST_UNDEF;
        vad_data->undef_count = 0; 
=======
>>>>>>> 53490523c80692c137dbadf19bdafc7a819ca7f4
      break;

    case ST_SILENCE:
    //Comprovació: si supera el llindar de veu (alfa1)
      if (f.p > vad_data->p1 + alfa1) {
        vad_data->state = ST_UNDEF;
        vad_data->undef_count = 0;
<<<<<<< HEAD
=======
>>>>>>> 0024cb8eee284967a209509eb86acb00a1c4b250
        vad_data->last_state_known = ST_SILENCE;
>>>>>>> 53490523c80692c137dbadf19bdafc7a819ca7f4
      }
      break;

    case ST_VOICE:
      //Comprovació: si no supera el llindar del silenci (alfa2)
      if (f.p < vad_data->p1 + alfa2) {
<<<<<<< HEAD
        vad_data->state = ST_UNDEF;  // Transició a l'estat undef
=======
        vad_data->state = ST_SILENCE; 
        vad_data->state = ST_UNDEF;
>>>>>>> 0024cb8eee284967a209509eb86acb00a1c4b250
        vad_data->undef_count = 0;
      }
      break;

    case ST_UNDEF:
<<<<<<< HEAD
    //Comprovar si és SILENCI a VEU
      // Si supera el llindar de silenci
      if (f.p > vad_data->p1 + alfa1) { 
                    
        if (vad_data->undef_count <= MAX_num_trames){
=======
<<<<<<< HEAD
    
    //Comprovar si es SILENCI ---> VEU
      if (f.p > vad_data->p0 + alpha1) { 
=======
    //Comprovar si es SILENCI ---> VEU
      if (f.p > vad_data->p1 + alfa1) { 
>>>>>>> 0024cb8eee284967a209509eb86acb00a1c4b250
        // Si supera el llindar de silenci 
            
        if (vad_data->undef_count < delta +1){
>>>>>>> 53490523c80692c137dbadf19bdafc7a819ca7f4
          vad_data->undef_count = vad_data->undef_count +1;
          vad_data->state = ST_UNDEF;
        }  
        else
          vad_data->state = ST_VOICE;
      } 
      else {
<<<<<<< HEAD
           vad_data->state = ST_SILENCE;
          
        } 
    //Comprovar si es  VEU ---> SILENCI
      if (f.p < vad_data->p0 + alpha2) {
=======
          vad_data->state = ST_SILENCE;         
      } 

    //Comprovar si és VEU a SILENCI
      // Si no supera el llindar de veu
      if (f.p < vad_data->p1 + alfa2) {
<<<<<<< HEAD
        
        if (vad_data->undef_count <= MAX_num_trames){
=======
>>>>>>> 0024cb8eee284967a209509eb86acb00a1c4b250
        // Si no supera el llindar de veu
        if (vad_data->undef_count < delta+1){
>>>>>>> 53490523c80692c137dbadf19bdafc7a819ca7f4
          vad_data->undef_count++;
          vad_data->state = ST_UNDEF;
        }  
        else
          vad_data->state = ST_SILENCE;
      } 
      else {
           vad_data->state = ST_VOICE;
      } 
<<<<<<< HEAD
      break;
  }

  return vad_data->state;  //Unicament retorna ST_UNDEF si ha expirat el temporitzador
=======

      break;
  }
    return vad_data->state;
<<<<<<< HEAD
=======
  //else return ST_UNDEF;
>>>>>>> 0024cb8eee284967a209509eb86acb00a1c4b250
>>>>>>> 53490523c80692c137dbadf19bdafc7a819ca7f4
}


void vad_show_state(const VAD_DATA *vad_data, FILE *out) {
  fprintf(out, "%d\t%f\n", vad_data->state, vad_data->last_feature);
}
