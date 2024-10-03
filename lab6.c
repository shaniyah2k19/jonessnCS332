#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//Define a structure to represent a listing with various properties
struct listing {
int id, host_id, minimum_nights, number_of_reviews, calculated_host_listings_count, availability_365;
char *host_name, *neighbourhood_group,*neighbourhood,*room_type;
float latitude, longitude, price;
};
//Function to parse a line from the CSV file and fill a listing struct
struct listing getfields(char* line){
    struct listing item;
    //Convert each file in the line seprated by commas
    item.id = atoi(strtok(line,","));
    item.host_id = atoi(strtok(NULL,","));
    item.host_name = strdup(strtok(NULL,","));
    item.neighbourhood_group = strdup(strtok(NULL,","));
    item.neighbourhood = strdup(strtok(NULL,","));
    item.latitude = atof(strtok(NULL,","));
    item.longitude = atof(strtok(NULL,","));
    item.room_type = strdup(strtok(NULL,","));
    item.price = atof(strtok(NULL,","));
    item.minimum_nights = atoi(strtok(NULL,","));
    item.number_of_reviews = atoi(strtok(NULL,","));
    item.calculated_host_listings_count = atoi(strtok(NULL,","));
    item.availability_365 = atoi(strtok(NULL,","));
    return item;
}
//Comparison functions for qsort to sorting listings by host name in alphabet order
int compareByHostName(const void *a, const void *b){
    struct listing *itemA = (struct listing *)a;
    struct listing *itemB = (struct listing *)b;
    return strcmp(itemA->host_name,itemB->host_name);
}
//Comparison functions for qsort to sorting listings by price numerically
int compareByPrice(const void *a, const void *b){
    struct listing *itemA = (struct listing *)a;
    struct listing *itemB = (struct listing *)b;
    return (itemA->price > itemB->price)-(itemA->price < itemB->price);
}
int main(){
    //Opening the CSV file for reading
    FILE *fptr = fopen("listings.csv","r");
    if(fptr == NULL){
        //If file couldn't be opened, print error message and return -1
        perror("Unable to open file!");
        return -1;
    }
    char line[1024];// Buffer to store each line from the file
    struct listing list_items[100000];//Array to store listings (maximum of 100,000)
    int count = 0;//Counting how many listings were read

    fgets(line, sizeof(line),fptr);//Reading the header line and discard it
    //Reading each line of the file and store the parsed listing in the array
    while(fgets(line,sizeof(line),fptr) != NULL){
        list_items[count++] = getfields(line);//Parsing and store the listing
    }
    fclose(fptr);//Close the file after reading all the data
    //Sorting the listings by host name using qsort anf the compareByHostName function
    qsort(list_items,count,sizeof(struct listing),compareByHostName);
    //Opening a file to store the sorted listings by host name
    FILE *sorted_by_host_name = fopen("sorted_by_host_name.csv","w");
    if(sorted_by_host_name == NULL){
        perror("Unable to open file for writing!");
        return -1;
    }
    //Writing the sorted listings to the file
    for(int i = 0; i < count; i++){
        fprintf(sorted_by_host_name,"%d,%d,%s,%s,%s,%f,%f,%s,%f,%d,%d,%d,%d\n",
        list_items[i].id,list_items[i].host_id,list_items[i].host_name,
        list_items[i].neighbourhood_group,list_items[i].neighbourhood,
        list_items[i].latitude,list_items[i].longitude,list_items[i].room_type,
        list_items[i].price,list_items[i].minimum_nights,list_items[i].number_of_reviews,
        list_items[i].calculated_host_listings_count,list_items[i].availability_365);
    }
    fclose(sorted_by_host_name);//Closing the file after writing
    //Opening a file to store the sorted listings by price
    qsort(list_items,count,sizeof(struct listing),compareByPrice);
    FILE *sorted_by_price = fopen("sorted_by_price.csv","w");
    if(sorted_by_price == NULL){
        perror("Unable to open file for writing!");
        return -1;
    }
    //Write the sorted listings to the file
    for(int i = 0; i < count; i++){
        fprintf(sorted_by_price,"%d,%d,%s,%s,%s,%f,%f,%s,%f,%d,%d,%d,%d\n",
        list_items[i].id,list_items[i].host_id,list_items[i].host_name,
        list_items[i].neighbourhood_group,list_items[i].neighbourhood,
        list_items[i].latitude,list_items[i].longitude,list_items[i].room_type,
        list_items[i].price,list_items[i].minimum_nights,list_items[i].number_of_reviews,
        list_items[i].calculated_host_listings_count,list_items[i].availability_365);
    }
    fclose(sorted_by_price);//Closing the file after writing
    return 0;//Indicate program completed successfully

}

