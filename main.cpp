#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <recount.h>
#include <algorithm>
#include <cstring>
#include <sys/time.h>
#include <omp.h>
#include <QDir>

using namespace std;

//PROTOTYPES
void calculate_p_participation(vector<recount> &content, string mode="sequential");
void calculate_p_abstention(vector<recount> &content, string mode="sequential");
int calculate_total_census(vector<recount> content, string comp_mode="sequential");
int calculate_total_abstention(vector<recount> content, string comp_mode="sequential");
recount get_min_census(vector<recount> content, string comp_mode="sequential");
recount get_max_census(vector<recount> content, string comp_mode="sequential");
float get_mean_p_participation(vector<recount> content, string comp_mode="sequential");
float get_mean_p_abstention(vector<recount> content, string comp_mode="sequential");


//MAIN
int main()
{
    // It can be used to know the execution path
    //cout << "[EXECUTION PATH]: " << QDir::currentPath().toStdString() + '\n';
    // Path and name of the csv file
   string fname = "../project_base/elecciones_navarra_1999.csv";

    // Init variables
    vector<recount> content;
    vector<string> row;
    recount read_recount = recount();
    int count_row = 0;
    string line, word;


    //Read document
    ifstream file (fname, ios::in);
    if(file.is_open())
    {
        //Read each row of the file
        while(getline(file, line))
        {
            row.clear(); // Clear row array
            stringstream stream(line);
            // Divide the line by the separator (;) and each value is an element of the array row
            while(getline(stream, word, ';'))
                row.push_back(word);
            if (count_row>0){ //First row of the document correspond to the header
                // TO_DO: CREATE recount object and append to content
                int code =stoi(row[0]);
                string municipal = row[1];
                int zone = stoi(row[2]);
                int section = stoi(row[3]);
                string nombre = row[4];
                const char* name = nombre.c_str();
                int census = stoi(row[5]);
                int abstentions = stoi(row[6]);
                int v_cast = stoi(row[7]);
                int v_invalid = stoi(row[8]);
                int v_valid = stoi(row[9]);
                int v_white = stoi(row[10]);

                float p_abstention;
                float p_participation;


               recount pueblo(code,municipal,zone,section,*name,census,abstentions,v_cast,v_invalid,v_valid,v_white,p_participation,p_abstention);


                content.push_back(pueblo);

                //read_recount.print(); //To see the information uncomment this line
            }
            count_row++;
        }
        cout<<"[OK]: "<< count_row << " lines read from the file and added to the list of contents" << "\n\n\n\n";
    }
    else{
        cout<<"[ERROR]: Could not open the file, check the file path\n";
    }

    //TO_DO: CALL TO FUNCTIONS AND MEASURE TIMES


    cout<<"||||||||||||||||||||||ASIGNACION DEL PORCENTAJE DE PARTICIPACION||||||||||||||||||||||\n\n\n";
    calculate_p_participation(content,"sequential");
    calculate_p_participation(content,"parallel");



    cout<<"||||||||||||||||||||||TOTAL CENSUS||||||||||||||||||||||\n\n\n";
    calculate_total_census(content,"sequential");
    calculate_total_census(content,"parallel");



    cout<<"||||||||||||||||||||||ASIGNACION DEL PORCENTAJE DE ABSTENCIÓN||||||||||||||||||||||\n\n\n";

    calculate_p_abstention(content,"sequential");
    calculate_p_abstention(content,"parallel");

    cout<<"||||||||||||||||||||||TOTAL ABSTENCIONES||||||||||||||||||||||\n\n\n";


        calculate_total_abstention(content,"sequential");
        calculate_total_abstention(content,"parallel");


    cout<<"||||||||||||||||||||||CENSO MINIMO||||||||||||||||||||||\n\n\n";
    get_min_census(content,"sequential");
    get_min_census(content,"parallel");


    cout<<"||||||||||||||||||||||CENSO MAXIMO||||||||||||||||||||||\n\n\n";
    get_max_census(content,"sequential");
    get_max_census(content,"parallel");



    cout<<"||||||||||||||||||||||PORCENTAJE MEDIO DE PARTICIPACION||||||||||||||||||||||\n\n\n";

    get_mean_p_participation(content,"sequential");
    get_mean_p_participation(content,"parallel");

    cout<<"||||||||||||||||||||||PORCENTAJE MEDIO DE ABSTENCION||||||||||||||||||||||\n\n\n";

    get_mean_p_abstention(content,"sequential");
    get_mean_p_abstention(content,"parallel");









    //PRUEBA DE QUE SETEA BIEN LOS PORCENTAJES

        //cout<<"participacion"<<content[0].get_p_participation()<<endl;
        //cout<<"participacion"<<content[0].get_p_abstention()<<endl;






    return 0;
}//fin main

//TO_DO: FUNCTIONS

/**
 * @brief calculate_total_census calculate total number of census people
 * @param content vector of recount elements
 * @param comp_mode is the computational mode, it can be: sequential or parallel
 * @return number of census people in total
 */
int calculate_total_census(vector<recount> content, string comp_mode){
        // Init variables

        struct timeval begin, end;
        double wt1 = 0,wt2 = 0;
        int total = 0;
        //int census = 0;

        wt1=omp_get_wtime();
        gettimeofday(&begin, 0);
        // Sequential mode
        if(comp_mode=="sequential"){

            // TO_DO: calculate total in sequential mode

            for(int i = 0; i<content.size(); i++){
                total+=content[i].get_census();
            }




        // Parallel mode
        }else if(comp_mode=="parallel"){

            // TO_DO: calculate total in parallel mode

        #pragma omp parallel for reduction(+:total)
            for(int i = 0; i < content.size(); i++){
                total += content[i].get_census();
            }




        }else{
            printf("[ERROR]: Not valid computational mode in %s. Only: sequential and parallel\n", __FUNCTION__);
            throw ("ERROR");
        }
        gettimeofday(&end, 0);
        wt2=omp_get_wtime();

        //Print results;
        long seconds = end.tv_sec - begin.tv_sec;
        long microseconds = end.tv_usec - begin.tv_usec;
        double elapsed = seconds*1e-6 + microseconds; //in microseconds
        printf("[CPU_TIME]: %s \tComputational mode: %s  \t%.2f microseconds \n", __FUNCTION__, comp_mode.c_str(), elapsed);
        double omp_elapse = (wt2-wt1)*1e-6;
        printf("[OMP_TIME]: %s \tComputational mode: %s  \t%.2f microseconds \n\n\n\n\n", __FUNCTION__, comp_mode.c_str(), omp_elapse);
        printf("Numero total de censados : %d personas\n\n\n\n\n", total);
        return total;
}
void calculate_p_participation(vector<recount> &content, string comp_mode){




        // Init variables

        struct timeval begin, end;
        double wt1 = 0,wt2 = 0;
        float porcenParticipation=0;

        wt1=omp_get_wtime();
        gettimeofday(&begin, 0);
        // Sequential mode
        if(comp_mode=="sequential"){

            // TO_DO: calculate total in sequential mode

            for(int i=0; i<content.size(); i++){
                porcenParticipation = (content[i].get_v_cast()*100)/content[i].get_census();
                content[i].set_p_participation(porcenParticipation);



        // Parallel mode
        }
        }else if(comp_mode=="parallel"){

            // TO_DO: calculate total in parallel mode

                #pragma omp parallel for
                for(int i = 0; i < content.size(); i++){
                porcenParticipation = (content[i].get_v_cast() * 100.0) / content[i].get_census();
                content[i].set_p_participation(porcenParticipation);
}




        }else{
            printf("[ERROR]: Not valid computational mode in %s. Only: sequential and parallel\n", __FUNCTION__);
            throw ("ERROR");
        }
        gettimeofday(&end, 0);
        wt2=omp_get_wtime();

        //Print results;
        long seconds = end.tv_sec - begin.tv_sec;
        long microseconds = end.tv_usec - begin.tv_usec;
        double elapsed = seconds*1e-6 + microseconds; //in microseconds
        printf("[CPU_TIME]: %s \tComputational mode: %s  \t%.2f microseconds \n", __FUNCTION__, comp_mode.c_str(), elapsed);
        double omp_elapse = (wt2-wt1)*1e-6;
        printf("[OMP_TIME]: %s \tComputational mode: %s  \t%.2f microseconds \n\n\n\n\n", __FUNCTION__, comp_mode.c_str(), omp_elapse);




}





void calculate_p_abstention(vector<recount> &content, string comp_mode){


    // Init variables
    //recount r = recount();
    struct timeval begin, end;
    double wt1 = 0,wt2 = 0;
    float porcenAbst=0;

    wt1=omp_get_wtime();
    gettimeofday(&begin, 0);
    // Sequential mode
    if(comp_mode=="sequential"){

        // TO_DO: calculate total in sequential mode

        for(int i=0; i<content.size(); i++){
            porcenAbst = (content[i].get_abstentions()*100)/content[i].get_census();
            content[i].set_p_abstention(porcenAbst);



    // Parallel mode
    }
    }else if(comp_mode=="parallel"){

        // TO_DO: calculate total in parallel mode

            #pragma omp parallel for
            for(int i = 0; i < content.size(); i++){
            porcenAbst = (content[i].get_abstentions() * 100.0) / content[i].get_census();
            content[i].set_p_abstention(porcenAbst);


}




    }else{
        printf("[ERROR]: Not valid computational mode in %s. Only: sequential and parallel\n", __FUNCTION__);
        throw ("ERROR");
    }
    gettimeofday(&end, 0);
    wt2=omp_get_wtime();

    //Print results;
    long seconds = end.tv_sec - begin.tv_sec;
    long microseconds = end.tv_usec - begin.tv_usec;
    double elapsed = seconds*1e-6 + microseconds; //in microseconds
    printf("[CPU_TIME]: %s \tComputational mode: %s  \t%.2f microseconds \n", __FUNCTION__, comp_mode.c_str(), elapsed);
    double omp_elapse = (wt2-wt1)*1e-6;
    printf("[OMP_TIME]: %s \tComputational mode: %s  \t%.2f microseconds \n\n\n\n\n", __FUNCTION__, comp_mode.c_str(), omp_elapse);
}




int calculate_total_abstention(vector<recount> content, string comp_mode){
        // Init variables

        struct timeval begin, end;
        double wt1 = 0,wt2 = 0;
        int totalAbsten = 0;
        //int census = 0;

        wt1=omp_get_wtime();
        gettimeofday(&begin, 0);
        // Sequential mode
        if(comp_mode=="sequential"){

            // TO_DO: calculate total in sequential mode

            for(int i = 0; i<content.size(); i++){
                totalAbsten+=content[i].get_abstentions();
            }




        // Parallel mode
        }else if(comp_mode=="parallel"){

            // TO_DO: calculate total in parallel mode

        #pragma omp parallel for reduction(+:totalAbsten)
            for(int i = 0; i < content.size(); i++){
                totalAbsten+=content[i].get_abstentions();
            }




        }else{
            printf("[ERROR]: Not valid computational mode in %s. Only: sequential and parallel\n", __FUNCTION__);
            throw ("ERROR");
        }
        gettimeofday(&end, 0);
        wt2=omp_get_wtime();

        //Print results;
        long seconds = end.tv_sec - begin.tv_sec;
        long microseconds = end.tv_usec - begin.tv_usec;
        double elapsed = seconds*1e-6 + microseconds; //in microseconds
        printf("[CPU_TIME]: %s \tComputational mode: %s  \t%.2f microseconds \n", __FUNCTION__, comp_mode.c_str(), elapsed);
        double omp_elapse = (wt2-wt1)*1e-6;
        printf("[OMP_TIME]: %s \tComputational mode: %s  \t%.2f microseconds \n\n\n\n\n", __FUNCTION__, comp_mode.c_str(), omp_elapse);
        printf("Numero total de abstenciones : %d personas\n\n\n\n\n",totalAbsten);
        return totalAbsten;
}




recount get_min_census(vector<recount> content, string comp_mode){

    // Init variables

    struct timeval begin, end;
    double wt1 = 0,wt2 = 0;
    int minimo = INT_MAX;
    recount puebloMinimo;

    wt1=omp_get_wtime();
    gettimeofday(&begin, 0);
    // Sequential mode
    if(comp_mode=="sequential"){

        // TO_DO: calculate total in sequential mode

        for(int i = 0; i<content.size(); i++){
            if(content[i].get_census() < minimo){
                minimo=content[i].get_census();
                puebloMinimo=content[i];
            }
        }




    // Parallel mode
    }else if(comp_mode=="parallel"){

        // TO_DO: calculate total in parallel mode

        #pragma omp parallel
    {
    recount localPuebloMinimo;
    int localMinimo = INT_MAX;

    #pragma omp for nowait
    for(int i = 0; i < content.size(); i++){
        if(content[i].get_census() < localMinimo){
            localMinimo = content[i].get_census();
            localPuebloMinimo = content[i];
        }
    }

    #pragma omp critical
    {
        if(localMinimo < minimo){
            minimo = localMinimo;
            puebloMinimo = localPuebloMinimo;
        }
    }
}




    }else{
        printf("[ERROR]: Not valid computational mode in %s. Only: sequential and parallel\n", __FUNCTION__);
        throw ("ERROR");
    }
    gettimeofday(&end, 0);
    wt2=omp_get_wtime();

    //Print results;
    long seconds = end.tv_sec - begin.tv_sec;
    long microseconds = end.tv_usec - begin.tv_usec;
    double elapsed = seconds*1e-6 + microseconds; //in microseconds
    printf("[CPU_TIME]: %s \tComputational mode: %s  \t%.2f microseconds \n", __FUNCTION__, comp_mode.c_str(), elapsed);
    double omp_elapse = (wt2-wt1)*1e-6;
    printf("[OMP_TIME]: %s \tComputational mode: %s  \t%.2f microseconds \n\n\n\n\n", __FUNCTION__, comp_mode.c_str(), omp_elapse);
    printf("El pueblo con menos censados tiene %d personas \n\n\n\n\n", puebloMinimo.get_census());
    return puebloMinimo;


}

recount get_max_census(vector<recount> content, string comp_mode){

    // Init variables

    struct timeval begin, end;
    double wt1 = 0,wt2 = 0;
    int max = INT_MIN;
    recount puebloMax;

    wt1=omp_get_wtime();
    gettimeofday(&begin, 0);
    // Sequential mode
    if(comp_mode=="sequential"){

        // TO_DO: calculate total in sequential mode

        for(int i = 0; i<content.size(); i++){
            if(content[i].get_census() > max){
                max=content[i].get_census();
                puebloMax=content[i];
            }
        }




    // Parallel mode
    }else if(comp_mode=="parallel"){

        // TO_DO: calculate total in parallel mode

        #pragma omp parallel
    {
    recount localpuebloMax;
    int localMax = INT_MIN;

    #pragma omp for nowait
    for(int i = 0; i < content.size(); i++){
        if(content[i].get_census() > localMax){
            localMax = content[i].get_census();
            localpuebloMax = content[i];
        }
    }

    #pragma omp critical
    {
        if(localMax > max){
            max = localMax;
            puebloMax = localpuebloMax;
        }
    }
}




    }else{
        printf("[ERROR]: Not valid computational mode in %s. Only: sequential and parallel\n", __FUNCTION__);
        throw ("ERROR");
    }
    gettimeofday(&end, 0);
    wt2=omp_get_wtime();

    //Print results;
    long seconds = end.tv_sec - begin.tv_sec;
    long microseconds = end.tv_usec - begin.tv_usec;
    double elapsed = seconds*1e-6 + microseconds; //in microseconds
    printf("[CPU_TIME]: %s \tComputational mode: %s  \t%.2f microseconds \n", __FUNCTION__, comp_mode.c_str(), elapsed);
    double omp_elapse = (wt2-wt1)*1e-6;
    printf("[OMP_TIME]: %s \tComputational mode: %s  \t%.2f microseconds \n\n\n\n\n", __FUNCTION__, comp_mode.c_str(), omp_elapse);
    printf("El pueblo con mas censados tiene %d personas \n\n\n\n\n", puebloMax.get_census());
    return puebloMax;


}


float get_mean_p_participation(vector<recount> content, string comp_mode){


    // Init variables

    struct timeval begin, end;
    double wt1 = 0,wt2 = 0;
    float participacion=0;

    float media;

    wt1=omp_get_wtime();
    gettimeofday(&begin, 0);
    // Sequential mode
    if(comp_mode=="sequential"){
    for(int i=0; i<content.size(); i++){

        participacion+=content[i].get_p_participation();
    }
    media=participacion/content.size();
    }else if(comp_mode=="parallel"){

        // TO_DO: calculate total in parallel mode

        float participacion = 0.0;

        #pragma omp parallel for reduction(+:participacion)
        for(int i = 0; i < content.size(); i++){

            participacion += content[i].get_p_participation();
        }

        media = participacion / content.size();

    }else{
        printf("[ERROR]: Not valid computational mode in %s. Only: sequential and parallel\n", __FUNCTION__);
        throw ("ERROR");
    }
    gettimeofday(&end, 0);
    wt2=omp_get_wtime();

    //Print results;
    long seconds = end.tv_sec - begin.tv_sec;
    long microseconds = end.tv_usec - begin.tv_usec;
    double elapsed = seconds*1e-6 + microseconds; //in microseconds
    printf("[CPU_TIME]: %s \tComputational mode: %s  \t%.2f microseconds \n", __FUNCTION__, comp_mode.c_str(), elapsed);
    double omp_elapse = (wt2-wt1)*1e-6;
    printf("[OMP_TIME]: %s \tComputational mode: %s  \t%.2f microseconds \n\n\n\n\n", __FUNCTION__, comp_mode.c_str(), omp_elapse);
    printf("El porcentaje medio de participacion es de %.6f por ciento \n\n\n\n\n",media);
    return media;

}

float get_mean_p_abstention(vector<recount> content, string comp_mode){
    // Init variables

    struct timeval begin, end;
    double wt1 = 0,wt2 = 0;
    float abstecion=0;

    float media;

    wt1=omp_get_wtime();
    gettimeofday(&begin, 0);
    // Sequential mode
    if(comp_mode=="sequential"){
    for(int i=0; i<content.size(); i++){

        abstecion+=content[i].get_p_abstention();
    }
    media=abstecion/content.size();
    }else if(comp_mode=="parallel"){

        // TO_DO: calculate total in parallel mode

        float abstecion = 0.0;

        #pragma omp parallel for reduction(+:abstecion)
        for(int i = 0; i < content.size(); i++){

            abstecion += content[i].get_p_abstention();
        }

        media = abstecion / content.size();

    }else{
        printf("[ERROR]: Not valid computational mode in %s. Only: sequential and parallel\n", __FUNCTION__);
        throw ("ERROR");
    }
    gettimeofday(&end, 0);
    wt2=omp_get_wtime();

    //Print results;
    long seconds = end.tv_sec - begin.tv_sec;
    long microseconds = end.tv_usec - begin.tv_usec;
    double elapsed = seconds*1e-6 + microseconds; //in microseconds
    printf("[CPU_TIME]: %s \tComputational mode: %s  \t%.2f microseconds \n", __FUNCTION__, comp_mode.c_str(), elapsed);
    double omp_elapse = (wt2-wt1)*1e-6;
    printf("[OMP_TIME]: %s \tComputational mode: %s  \t%.2f microseconds \n\n\n\n\n", __FUNCTION__, comp_mode.c_str(), omp_elapse);
    printf("El porcentaje medio de abstencion es de %.6f por ciento \n\n\n\n\n",media);
    return media;
}
