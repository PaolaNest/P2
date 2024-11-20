#!/bin/bash

# Be sure that this file has execution permissions:
# Use the nautilus explorer or chmod +x run_vad.sh

# Establecemos que el código de retorno de un pipeline sea el del último programa con código de retorno
# distinto de cero, o cero si todos devuelven cero.
set -o pipefail

# asignar un valor de 5.1 por defecto a la variable de entorno alfa1 (el - es por sintaxis)
<<<<<<< HEAD
<<<<<<< HEAD
alfa1=${1:-5}
alfa2=${2:-5}
=======
alfa1=${1:-5.1}
alfa1=${2:-5.1}
>>>>>>> 0024cb8eee284967a209509eb86acb00a1c4b250
>>>>>>> 53490523c80692c137dbadf19bdafc7a819ca7f4
=======
alfa1=${1:-5}
alfa2=${2:-5}
>>>>>>> ef17026e659c415ed63285a1753985b0a0aa7b1f

# Write here the name and path of your program and database
DIR_P2=$HOME/PAV/P2
DB=$DIR_P2/db.v4
CMD="$DIR_P2/bin/vad -1 $alfa1 -2 $alfa2"


for filewav in $DB/*/*wav; do
#    echo
    echo "**************** $filewav ****************"
    if [[ ! -f $filewav ]]; then 
	    echo "Wav file not found: $filewav" >&2
	    exit 1
    fi

    filevad=${filewav/.wav/.vad}

    $CMD -i $filewav -o $filevad || exit 1

# Alternatively, uncomment to create output wave files
#    filewavOut=${filewav/.wav/.vad.wav}
#    $CMD $filewav $filevad $filewavOut || exit 1

done

scripts/vad_evaluation.pl $DB/*/*lab

exit 0
