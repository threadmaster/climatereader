#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct stationinfo {
    char stationid[6];
    char stateid[3];
};  

int main(){

    FILE *stationdata;
    char c;
    int  numlines;           // keeps up with lines in file
    int  iter;               // stationdata array index
    int  i;                  // index variable
    int  length, plength;    // used to keep up with no null length of active buffers 
    char buffer[200], processed[300];  // Utility buffers for sting processing
    const char *stationdelim = "|";    // Delimiter for the station.txt file
    struct stationinfo tempdata;       // Temporary variable to hold structure data

    /* Use system calls to move files to local disk */

    system("mkdir /tmp/pounds");
    system("mkdir /tmp/pounds/data");
    system("wget -O /tmp/pounds/data/station.txt -q http://theochem.mercer.edu/csc330/data/station.txt"); 
    system("wget -O /tmp/pounds/data/daily.txt -q http://theochem.mercer.edu/csc330/data/daily.txt");

    /* Open the station.txt file and determined how many lines are in the file */

    stationdata = fopen("/tmp/pounds/data/station.txt", "r");

    numlines = 0;
    while ( ( c = getc(stationdata) ) != EOF ) {
        if ( c == '\n' ) numlines++;
    }

    printf("There are %d lines.\n", numlines);

    /* Now create memory to hold the structures of information (stationid, stateid) */

    struct stationinfo stations[numlines];

    /* Rewind the file */

    rewind(stationdata);

    /* Read each line and parse for station id and state, place these in local variables. 
     * Initialize the temp sum and number of readings in the structure.  Copy the local varables
     * holding stationid and state into the structure, copy the structure into the array.
     */

    
    iter = 0;  // The length of the stationdata array

    // The next two lines clear out the buffers to prepare for string processing. //
    for (i=0; i<strlen(buffer); i++) *(&buffer[0]+i) = '\0';
    for (i=0; i<strlen(processed); i++) *(&processed[0]+i) = '\0';

    // This is the big loop that reads the file.  
    while ( (fgets(buffer, sizeof(buffer), stationdata)) != NULL) {
        // need to create new buffer so that pipes are converted to spaces
        length=0;
        plength=0;

        // Figure out how many spaces in the buffer are being used -- check for '\0' 
        while ( buffer[length] != '\0' && length < strlen(buffer)) { 
            length++;
        }

        // Now that we know the length of the buffer, write it to a processed array,
        // but every time you see a delimiter, expand it to a delimiter and a space.
        // This is to set things up for the string tokenizer.
        for (i=0; i<length; i++) {
            if (buffer[i] == *stationdelim ) {
                processed[plength]=buffer[i];
                processed[plength+1]=' ';
                plength=plength+2;
            }
            else {
                processed[plength]=buffer[i];
                plength++;
            } 
        }

        // Now we will start using strcpy and the string tokenizer to extract the info
        // that we need.
        
        strcpy(tempdata.stationid, strtok(processed, stationdelim));
        strtok(NULL,stationdelim);
        strtok(NULL,stationdelim);
        strtok(NULL,stationdelim);
        strtok(NULL,stationdelim);
        strtok(NULL,stationdelim);
        strtok(NULL,stationdelim);

        // This next section is a bit interesting. Since spaces were added after each delimeter,
        // the length of the stateid "string" is too long to fit into the stateit character string
        // inside the structure.  This has very nasty side effects.  To compensate for this we
        // clear the buffer that we are no longer using, copy the string containing the state id
        // into it, and then, if needed, delete the leading space  and copy the appropriate data into
        // the stateid character array.
 
        for (i=0; i<strlen(buffer); i++) *(&buffer[0]+i) = '\0';

        strcpy(buffer, strtok(NULL, stationdelim));
        if ( buffer[0] != ' ' ) {
            tempdata.stateid[0] = buffer[0]; tempdata.stateid[1] = buffer[1]; tempdata.stateid[2] = '\0';
        }
        else {
            tempdata.stateid[0] = buffer[1]; tempdata.stateid[1] = buffer[2]; tempdata.stateid[2] = '\0';
        }

        // Now that the stationdata and stateid are in the proper format, copy then into the stations array
        // and increment the array index.
     
        stations[iter] = tempdata;
        iter++;

        // Clear the buffers before starting again.
        //
        for (i=0; i<strlen(buffer); i++) *(&buffer[0]+i) = '\0';
        for (i=0; i<strlen(processed); i++) *(&processed[0]+i) = '\0';
    }

    /* Done with the file, so close it */

    close(stationdata);

    /* See if it works. */

    for (i=0; i<numlines; i++ ){
        printf( "%s %s \n", stations[i].stationid, stations[i].stateid ); 
    } 

    /* Delete the files from disk */

    system("rm -rf /tmp/pounds");

}
