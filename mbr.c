//Bucur Ionut-Cristian,313CA
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main(int argc, char *argv[]){//declaram functia main incat sa ii putem parsa argumente si implicit sa ne folosim de ele

//TASK1
if(strcmp("1",argv[1])==0){//verificam daca taskul verficat este 1
FILE *fp;//fiser
fp=fopen("mbr.bin","rb");//deschidem fisierul necesar in modul citire binara
int nr_partitii,pozitie=4,ok=0,adresa_partie_curenta,total_space=0,MAXF,MAXD,adresa_partie_anterioara=0,used_space,i,k,asd;
char c[7],ce[8];


fseek(fp,0,0);//mutam cursorul la inceputul fisierului
fread(&nr_partitii,sizeof(int),1,fp);//dupa care citim numarul de paritii din mbr


for(i=0;i<nr_partitii;i++){//ne folosim de acesta pentru a trece prin toate partiile cu ajutorul acestui for
	fseek(fp,pozitie,0);//punem cursorul la inceputul fisierului
	fread(&adresa_partie_curenta,sizeof(int),1,fp);//citim adresa partitiei curente
	if(fseek(fp,adresa_partie_curenta,0)!=0){//verificam daca adresa este una valida ce apartine fisierului
		printf("INVALID\n");//in caz ca nu afisam INVALID
		break;//si oprim forul
	}
	fseek(fp,adresa_partie_curenta+4,0);//verificam daca numele partitie este de forma corecta si adica /dev/sd
	fread(&c,7,1,fp);
	c[7]='\0';//rezolvam "problema" freadului ce mi-a mancat o mare parte din sanatatea mintala aka ii punem terminator de sir stringului citit


	if(strcmp(c,"/dev/sd")!=0){//verificam daca numele partitiei actuale incepe cu /dev/sd
		printf("INVALID\n");//in caz contrar afisam INVALID
		break;
		}
	ok++;
	pozitie=pozitie+4;
}

if(ok==nr_partitii){//verificam daca numarul de partii valide este egal cu numarul de partitii
	pozitie=4;
	printf("SUCCES\n");//afisam mesajul aferent acestui lucru
	for(i=0;i<nr_partitii;i++){
		fseek(fp,pozitie,0);//resetam cursorul pentru a parcurge din nou partiile
		fread(&adresa_partie_curenta,sizeof(int),1,fp);//citim adresa partitiei curente
		used_space=0;//initializam cu 0 used_space
		if(i!=0){//lasam prima pe mai tarziu

			for(k=adresa_partie_anterioara+20;k<adresa_partie_curenta;k=k+12){//parcurgem partitia anterioara
				fseek(fp,k,0);
				fread(&asd,sizeof(int),1,fp);//sarim peste descrierea partitiei si verificam daca exista un folder sau un fisier acolo

				if(asd!=0){//folosindu-ne de faptul ca daca nu exista lungimea numelui e 0
					used_space=used_space+12;//in caz ca exista adunam spatiul ocupat de acestea in used_space
				
				}

			}
			printf("%d ",adresa_partie_curenta-adresa_partie_anterioara-20);//afisam total spaceul care matematic este egal cu adresa_partie_curenta-adresa_partie_anterioara-20
			printf("%d",used_space);
			printf("\n");
			
		}
		
		fseek(fp,adresa_partie_curenta+4,0);//mutam cursorul la numele paritiei
		fread(&ce,8,1,fp);//il citim
		ce[8]='\0';//rezolvam din nou problema FREAD :(
		printf("%s ",ce);//afiseaza nume partitie /dev/sd(blabla)
		
		
		adresa_partie_anterioara=adresa_partie_curenta;//adresa paritiei curente devine anterioara
		pozitie=pozitie+4;//trecem la urmatoarea partitie


	
	}
	fseek(fp,0,2);//ducem cursoul la final pt synergia cu ftell
	printf("%li ",ftell(fp)-adresa_partie_anterioara-20);//face verificare separat pt ultima
	used_space=0;
	for(k=adresa_partie_anterioara+20;k<ftell(fp);k=k+12){//parcurgem de la ultima partitie pana la final
		fseek(fp,k,0);
		fread(&asd,sizeof(int),1,fp);
		
		if(asd!=0){
			used_space=used_space+12;
	
		}
		fseek(fp,0,2);//readucem la final
	}
printf("%d\n",used_space);	
}	
fclose(fp);//inchidem fisieru'
}

//TASK2

if(strcmp("2",argv[1])==0){
	FILE *fp;//fisier bin
	FILE *fp1;//fiser operatii
	fp1=fopen("mbr.op","r");//le deschidem
	fp=fopen("mbr.bin","r+b");//r+ pentru a putea citi,dar si scrie
	while(1){//infinit pentru a citi si executa toate comenzile din mbr.op
		if(feof(fp1)){ //conditie de oprire,atingerea finalului
         		break;
		}
		char comanda[100],argument[100],partitie[100],nume_partitie_del[10],nume_part_curenta[10],*p,nume_folder_del[10];
		int pozitie=4,adresa_partie_curenta,i,j,nr_partitii,ok=0,MAXD,MAXF,pozitie2;

		fscanf(fp1,"%s",comanda);//citim comanda din mbr.op
		
		//DELETE_FILE
		if(strcmp("DELETE_FILE",comanda)==0){//verificam daca asta e comanda curenta
			fscanf(fp1,"%s",argument);//citim argumentul aferent acesteia
			char nume_partitie_del[10];
			memcpy(nume_partitie_del,argument,8);//scoatem partitia din path
			nume_partitie_del[8]='\0';//same shit ca la fread
			fseek(fp,0,0);//mutam cursoru la inceput
			fread(&nr_partitii,sizeof(int),1,fp);//citim nr partii
			for(i=0;i<nr_partitii;i++){//le parcurgem
				fseek(fp,pozitie,0);
				fread(&adresa_partie_curenta,sizeof(int),1,fp);//luam adresa partitiei curente
				fseek(fp,adresa_partie_curenta+4,0);//cauta partitia respectiva /dev/sd(blabla)
				fread(&nume_part_curenta,8,1,fp);// citim numele partiei curente
				nume_part_curenta[8]='\0';// mda fread strikes again

				pozitie=pozitie+4;//intializam pozitia 
				fread(&MAXD,sizeof(int),1,fp);//citim MAXD-ul curent
				fread(&MAXF,sizeof(int),1,fp);//citim MAXF-ul curent
				if(strcmp(nume_partitie_del,nume_part_curenta)==0){//verificam daca fisierul de sters se afla in aceasta partie
					
					p=strtok(argument,"/");//daca da ;;bin
					p=strtok(NULL,"/");//spargem pathul ;;sd(bla)
					p=strtok(NULL,"/");//teoretic folder aici sau fisier daca e direct pe partie
					strcpy(nume_folder_del,p);//verificam daca e fisier sau folder
					p=strtok(NULL,"/");

						if(p==NULL){//in caz fisierul e direct pe partie
							int kl=MAXF,i1,zero=0;

							char chestie[20];
							pozitie2=adresa_partie_curenta+20+(MAXD+(MAXD*MAXF))*12+4;//setam pozitia incat sa sarim peste descrierea partitiei si peste toate folderele
							fseek(fp,pozitie2,0);//sarim
							while(kl){//trecem prin toate fisierele

								fscanf(fp,"%s",chestie);//citim numele fisierului curent
								
								if(strcmp(chestie,nume_folder_del)==0){//daca este cel cautat
									fseek(fp,pozitie2-4,0);//mutam cursorul la inceputul descrierii fisierului

									for(i1=0;i1<3;i1++){//bagam 0 pe acolo sa marcam stergerea
									fwrite(&zero,1,sizeof(int),fp);	
									}
									ok=1;//setam faptul ca s-a executat comanda
								}

								pozitie2=pozitie2+12;//sarim la urmatorul fisier
								fseek(fp,pozitie2,0);
								kl--;//scadem numarul de fisiere suspecte
							}
						}
						if(p!=NULL){//in caz ca fisieru e intr-un folder 
							int kl=MAXD,i1,zero=0;

							char nume1[20],nume_fis_del[10];
							strcpy(nume_fis_del,p);//salvam numele fisierului
							pozitie2=adresa_partie_curenta+20+4;
							fseek(fp,pozitie2,0);
							while(kl){//trecem prin toate folderele

								fscanf(fp,"%s",nume1);

								if(strcmp(nume1,nume_folder_del)==0){//daca asta e numele fisierului
									fseek(fp,pozitie2+12,0);//sarim pe fisier

									for(i1=0;i1<3;i1++){//bagam 0 pe acolo sa marcam stergerea
									fwrite(&zero,1,sizeof(int),fp);	
									}
									ok=1;//setam corespunzator afisaju
								}

								pozitie2=pozitie2+12;//sarim la urmatorul folder
								fseek(fp,pozitie2,0);
								kl--;//scadem numarul de foldere suspecte 
							}
						}
					
				}
			}
			if(ok==1){
				printf("SUCCES\n");
			}
			if(ok==0){
				printf("INVALID\n");
			}
		
		}



		//DELETE_DIR
		if(strcmp("DELETE_DIR",comanda)==0){
			fscanf(fp1,"%s",argument);
			char nume_partitie_del[10];//scoatem partitia din path
			int ok=0;
			memcpy(nume_partitie_del,argument,8);//salvam numele paritiei
			nume_partitie_del[8]='\0';//end string
			fseek(fp,0,0);
			fread(&nr_partitii,sizeof(int),1,fp);//scoatem numarul de partii
			for(i=0;i<nr_partitii;i++){//trecem prin ele
				fseek(fp,pozitie,0);
				fread(&adresa_partie_curenta,sizeof(int),1,fp);//citim adresa partiei curente
				fseek(fp,adresa_partie_curenta+4,0);//setam cursoru'
				fread(&nume_part_curenta,8,1,fp);//citim numele adresei curente
				nume_part_curenta[8]='\0';// end string

				pozitie=pozitie+4;//sarim la urmatoarea partitie
				fread(&MAXD,sizeof(int),1,fp);//citim maxd curent
				fread(&MAXF,sizeof(int),1,fp);//citim si maxfu curent
				if(strcmp(nume_partitie_del,nume_part_curenta)==0){//verificam si intram in partitia din path
					
					p=strtok(argument,"/");//dev
					p=strtok(NULL,"/");//sd
					p=strtok(NULL,"/");//folder

					int pozitie2,i1,zero=0,kl=MAXD;
					char nume1[20];
					
					
					if(p!=NULL){//in caz ca chiar exista folderu in path
						pozitie2=adresa_partie_curenta+20+4;//sarim direct
						fseek(fp,pozitie2,0);
						strcpy(nume_folder_del,p);//verificam daca e fisier sau folder
						while(kl){//trecem prin toate folderele
							fread(&nume1,8,1,fp);//citim nume folderului curent
							nume1[8]='\0';

							if(strcmp(nume1,nume_folder_del)==0){//daca este cel cautat
								fseek(fp,(pozitie2-4),0);//setam cursoru incat sa-l stergem de tot
								for(i1=0;i1<(3+3*MAXF);i1++){//bagam 0 pe acolo sa marcam stergerea
									fwrite(&zero,1,sizeof(int),fp);	
								}
								ok=1;//succes
							}

							pozitie2=pozitie2+12;//sarim la urmatoru folder
							fseek(fp,pozitie2,0);//setam cursoru
							kl--;//actualizam lista foldere suspecte
						}
					}
				}
			}
			if(ok==1){
				printf("SUCCES\n");//daca s-a executat stergerea afisam ce trebuie
			}
			if(ok==0){
				printf("INVALID\n");//daca nu,afisam ce trebuie
			}
		}



		//RENAME_DIR
		if(strcmp("RENAME_DIR",comanda)==0){
			char nume_nou[10];
			fscanf(fp1,"%s",argument);//citim pathul cu fiseru'
			fscanf(fp1,"%s",nume_nou);//si numele nou
			char nume_partitie_del[10];//scoatem partitia din path
			int ok=0,lungime_nume_nou=strlen(nume_nou);
			memcpy(nume_partitie_del,argument,8);//salvam partia
			nume_partitie_del[8]='\0';
			fseek(fp,0,0);//setam cursoru
			fread(&nr_partitii,sizeof(int),1,fp);//citim nru de partitii
			for(i=0;i<nr_partitii;i++){//parcurgem toate partiile
				fseek(fp,pozitie,0);
				fread(&adresa_partie_curenta,sizeof(int),1,fp);
				fseek(fp,adresa_partie_curenta+4,0);
				fread(&nume_part_curenta,8,1,fp);
				nume_part_curenta[8]='\0';

				pozitie=pozitie+4;
				fread(&MAXD,sizeof(int),1,fp);
				fread(&MAXF,sizeof(int),1,fp);
				if(strcmp(nume_partitie_del,nume_part_curenta)==0){//verificam daca partitia e cea din path
					
					p=strtok(argument,"/");
					p=strtok(NULL,"/");
					p=strtok(NULL,"/");

					int pozitie2,i1,kl=MAXD;
					char nume1[20];
					
					
					if(p!=NULL){
						pozitie2=adresa_partie_curenta+20+4;
						fseek(fp,pozitie2,0);
						strcpy(nume_folder_del,p);//verificam daca e fisier sau folder
						while(kl){//trecem prin toate fisierele
							fread(&nume1,8,1,fp);
							nume1[8]='\0';

							if(strcmp(nume1,nume_folder_del)==0){//verificam daca este numele suspectului
								fseek(fp,(pozitie2-4),0);//setam cursuru
								fwrite(&lungime_nume_nou,1,sizeof(int),fp);//introducem lungimea numelui nou
								fwrite(&nume_nou,1,8,fp);//introducem numele nou
								ok=1;//setam contoru'
							}

							pozitie2=pozitie2+12;//sarim la urmatoru director
							fseek(fp,pozitie2,0);//chiar asa
							kl--;//reducem cercul suspectilor
						}
					}
				}
			}
			if(ok==1){
				printf("SUCCES\n");//daca s-a executat stergerea afisam ce trebuie
			}
			if(ok==0){
				printf("INVALID\n");//daca nu,afisam ce trebuie
			}
		}




		//RENAME_FILE
		if(strcmp("RENAME_FILE",comanda)==0){
			char nume_nou[10],nume_partitie_del[10];
			fscanf(fp1,"%s",argument);//citim pathu cu numele vechi al fisierului
			fscanf(fp1,"%s",nume_nou);//citim numele nou al acestuia
			int lungime_nume_nou=strlen(nume_nou),ok=0;
			memcpy(nume_partitie_del,argument,8);//salvam numele paritiei in care se afla acesta sau in care ar trebui sa se afle
			nume_partitie_del[8]='\0';
			fseek(fp,0,0);
			fread(&nr_partitii,sizeof(int),1,fp);//luam numarul de partii
			for(i=0;i<nr_partitii;i++){//le parcurgem
				fseek(fp,pozitie,0);
				fread(&adresa_partie_curenta,sizeof(int),1,fp);
				fseek(fp,adresa_partie_curenta+4,0);
				fread(&nume_part_curenta,8,1,fp);
				nume_part_curenta[8]='\0';

				pozitie=pozitie+4;
				fread(&MAXD,sizeof(int),1,fp);
				fread(&MAXF,sizeof(int),1,fp);
				if(strcmp(nume_partitie_del,nume_part_curenta)==0){//am gasit partitia buna
					
					p=strtok(argument,"/");//dev
					p=strtok(NULL,"/");//sd
					p=strtok(NULL,"/");//folder/fisier
					strcpy(nume_folder_del,p);
					p=strtok(NULL,"/");

						if(p==NULL){//in caz ca fisieru' e direct pe partitie
							int kl=MAXF,i1,zero=0;

							char chestie[20];
							pozitie2=adresa_partie_curenta+20+(MAXD+(MAXD*MAXF))*12+4;//sarim direct in fisiere,peste descriere partitie si foldere
							fseek(fp,pozitie2,0);
							while(kl){//trecem prin toate fisierele

								fscanf(fp,"%s",chestie);
								
								if(strcmp(chestie,nume_folder_del)==0){//daca e cel cautat
									fseek(fp,(pozitie2-4),0);

									fwrite(&lungime_nume_nou,1,sizeof(int),fp);//bagam lungimea numelui noi
									fwrite(&nume_nou,1,8,fp);//bagam numele nou
									ok=1;//setam mesaju de final
								}

								pozitie2=pozitie2+12;//sarim la urmatoru fisier
								fseek(fp,pozitie2,0);
								kl--;
							}
						}
						if(p!=NULL){//in caz ca fisieru e intr-un folder
							int kl=MAXD,i1,zero=0;

							char nume1[20],nume_fis_del[20];
							strcpy(nume_fis_del,p);
							pozitie2=adresa_partie_curenta+20+4;
							fseek(fp,pozitie2,0);
							while(kl){//trecem prin toate folderele

								fscanf(fp,"%s",nume1);

								if(strcmp(nume1,nume_folder_del)==0){//daca este numele fisierului cautat
									fseek(fp,pozitie2+8,0);

									fwrite(&lungime_nume_nou,1,sizeof(int),fp);//bagam lungimea noua
									fwrite(&nume_nou,1,8,fp);//si numele nou
									ok=1;
								}

								pozitie2=pozitie2+12;//sarim la urmatoru
								fseek(fp,pozitie2,0);
								kl--;
							}
						}
					
				}
			}
			if(ok==1){
				printf("SUCCES\n");
			}
			if(ok==0){
				printf("INVALID\n");
			}
		
		}



		//PRINT
		if(strcmp("PRINT",comanda)==0){
			char nume_nou[10],nume_partitie_del[10];
			fscanf(fp1,"%s",argument); //din path luam pathul cu fisieru


			int ok=0,lung_nume=0;
			memcpy(nume_partitie_del,argument,8);//salvam paritia coresp
			nume_partitie_del[8]='\0';
			fseek(fp,0,0);
			fread(&nr_partitii,sizeof(int),1,fp);
			for(i=0;i<nr_partitii;i++){//parcurgem paritiile
				fseek(fp,pozitie,0);
				fread(&adresa_partie_curenta,sizeof(int),1,fp);
				fseek(fp,adresa_partie_curenta+4,0);
				fread(&nume_part_curenta,8,1,fp);
				nume_part_curenta[8]='\0';

				pozitie=pozitie+4;
				fread(&MAXD,sizeof(int),1,fp);
				fread(&MAXF,sizeof(int),1,fp);
				if(strcmp(nume_partitie_del,nume_part_curenta)==0){//am gasit partia
					
					p=strtok(argument,"/");
					p=strtok(NULL,"/");
					p=strtok(NULL,"/");

					int pozitie2=0,i1,zero=0,kl=MAXD,byti;
					char nume1[10],asd[MAXF];
					
					
					if(p!=NULL){//daca pathu e bun
						pozitie2=adresa_partie_curenta+20+4;
						fseek(fp,pozitie2,0);
						strcpy(nume_folder_del,p);//salvam numele
						while(kl){//trecem prin toate folderele
							fread(&nume1,8,1,fp);
							nume1[8]='\0';
							if(strcmp(nume1,nume_folder_del)==0){//daca e folderu cautat
								ok=1;
								for(i1=0;i1<MAXF;i1++){
									fseek(fp,(pozitie2+12),0);
									char nume_fis[10];
									
									

									fseek(fp,(pozitie2+12),0);//sarim
									fread(nume_fis,8,1,fp);//si citim numele fisierului
									nume_fis[8]='\0';
									
									if(strlen(nume_fis)>1){//rezolvare un bug 
										if((strlen(nume_fis))<3){
											fseek(fp,(pozitie2+8),0);//citim nume fisier corespunzator
											fread(nume_fis,8,1,fp);
											nume_fis[8]='\0';
											puts(nume_fis);//afisam fisier
										}
										else{puts(nume_fis);}//afisam fisier
									}
									//printf("%s\n",nume_fis);}
									
									pozitie2=pozitie2+12;//sarim la urmatoru folder
								}
							}

							pozitie2=pozitie2+12;
							fseek(fp,pozitie2,0);
							kl--;//eliminam din suspecti
						}
					}
				}
			
			}
			if(ok==0){printf("INVALID\n");}//afisam mesaj in caz ca n-a reusit treaba

		}


		//CREATE_DIR
		if(strcmp("CREATE_DIR",comanda)==0){
			char nume_partitie_del[10];
			fscanf(fp1,"%s",argument);//citim path+nume folder
			int ok1=1;

			memcpy(nume_partitie_del,argument,8);//salvam numele partiei
			nume_partitie_del[8]='\0';
			fseek(fp,0,0);
			fread(&nr_partitii,sizeof(int),1,fp);
			for(i=0;i<nr_partitii;i++){//cautam prin toate partitiile
				fseek(fp,pozitie,0);
				fread(&adresa_partie_curenta,sizeof(int),1,fp);
				fseek(fp,adresa_partie_curenta+4,0);
				fread(&nume_part_curenta,8,1,fp);
				nume_part_curenta[8]='\0';

				pozitie=pozitie+4;
				fread(&MAXD,sizeof(int),1,fp);
				fread(&MAXF,sizeof(int),1,fp);
				if(strcmp(nume_partitie_del,nume_part_curenta)==0){//am gasit partitia buna
					
					p=strtok(argument,"/");//dev
					p=strtok(NULL,"/");//sd
					p=strtok(NULL,"/");//folder
					strcpy(nume_folder_del,p);//salvam numele folderului
					p=strtok(NULL,"/");

						if(p==NULL){//in caz ca pathul e bun
							int kl=MAXD,i1,zero=0,lung_nume_folder=strlen(nume_folder_del);


							int lung_nume_cur;
							char chestie[10];
							pozitie2=adresa_partie_curenta+20;//sarim peste descriere partie direct la foldere
							fseek(fp,pozitie2,0);
							while(kl){//trecem prin toate folderele
								fscanf(fp,"%s",chestie);
								if(strcmp(chestie,nume_folder_del)==0){//daca exista deja
									ok1=0;//s-a dus tot
									break;
									
								}
								fseek(fp,pozitie2,0);
								fread(&lung_nume_cur,sizeof(int),1,fp);//lungimea numelui folderlui curent
								if(lung_nume_cur==0){//daca e 0 inseamna ca e loc aici
									ok=1;//SUCCES
									fseek(fp,pozitie2,0);//setam cursor
									fwrite(&lung_nume_folder,1,sizeof(int) ,fp);//bagam lungimea numelui
									fwrite(nume_folder_del,1,lung_nume_folder,fp);//bagam numele
								
								}
								pozitie2=pozitie2+12;//sarim la urmatorul loc
								fseek(fp,pozitie2,0);
								kl--;
							}
						}

					
				}
			}
			if(ok==1){
				printf("SUCCES\n");
			}
			if(ok==0){
				printf("INVALID\n");
			}
		
		}



		//CREATE_FILE
		if(strcmp("CREATE_FILE",comanda)==0){

			fscanf(fp1,"%s",argument);//luam pathul si numele fisierului
			char nume_partitie_del[10];
			memcpy(nume_partitie_del,argument,8);//scoatem partitia din path
			nume_partitie_del[8]='\0';
			fseek(fp,0,0);
			fread(&nr_partitii,sizeof(int),1,fp);
			for(i=0;i<nr_partitii;i++){//parcurgem partiile
				fseek(fp,pozitie,0);
				fread(&adresa_partie_curenta,sizeof(int),1,fp);
				fseek(fp,adresa_partie_curenta+4,0);
				fread(&nume_part_curenta,8,1,fp);
				nume_part_curenta[8]='\0';

				pozitie=pozitie+4;
				fread(&MAXD,sizeof(int),1,fp);
				fread(&MAXF,sizeof(int),1,fp);
				if(strcmp(nume_partitie_del,nume_part_curenta)==0){//am gasit partitia
					
					p=strtok(argument,"/");//dev
					p=strtok(NULL,"/");//sd
					p=strtok(NULL,"/");//folder/nume fisier
					strcpy(nume_folder_del,p);//verificam daca e fisier sau folder
					p=strtok(NULL,"/");

						if(p==NULL){//in caz ca e pe partitie
							int kl=MAXF,i1,ok1=1,lung_nume_folder=strlen(nume_folder_del);

							char chestie[20],lung_nume_cur[10];
							pozitie2=adresa_partie_curenta+20+(MAXD+(MAXD*MAXF))*12+4;
							fseek(fp,pozitie2,0);
							while(kl){//trecem prin toate fisierele

								fscanf(fp,"%s",chestie);
								
								if(strcmp(chestie,nume_folder_del)==0){//fisier deja existent
									ok1=0;
								}
								if(ok1==0){//oprim operationea
									break;
								}
								fseek(fp,pozitie2,0);
								fread(&lung_nume_cur,sizeof(int),1,fp);//lungimea numelui fisierului
								if(lung_nume_cur==0){//avem loc aici
									ok=1;
									fseek(fp,pozitie2-4,0);//bagam lungimea numelui si numele
									fwrite(&lung_nume_folder,1,sizeof(int) ,fp);
									fwrite(&nume_folder_del,1,8,fp);
								
								}
								fseek(fp,pozitie2,0);							
								pozitie2=pozitie2+12;//sarim mai departe
								fseek(fp,pozitie2,0);
								kl--;
							}
							if(ok1==0){
								break;
							}
						}
						if(p!=NULL){//in caz ca fisieru e intr-un folder
							char nume1[20],nume_fis_del[10];
							int kl=MAXD,i1,ok1=1,lung_nume_cur,i3,pozitie,pozitie3,lung_nume_folder;
							
							
							strcpy(nume_fis_del,p);
							int lung_nume_fis=strlen(nume_fis_del);
							pozitie2=adresa_partie_curenta+20+4;
							fseek(fp,pozitie2,0);
							while(kl){//trecem prin toate folderele
								fseek(fp,pozitie2,0);
								fread(nume1,8,1,fp);
								nume1[8]='\0';


								if(strcmp(nume1,nume_folder_del)==0){//verificam daca e folderu bun
									pozitie3=pozitie2+4;
									fseek(fp,pozitie3,0);

									for(i3=0;i3<MAXF;i3++){//cautam loc pt fisier
										fseek(fp,pozitie3+4,0);
										fread(&lung_nume_cur,sizeof(int),1,fp);
										if(lung_nume_cur==0){//l-am gasit
											ok=1;
											fseek(fp,(pozitie3),0);//bagam lungimea si numele
											fwrite(&lung_nume_fis,1,sizeof(int),fp);
											fwrite(&nume_fis_del,1,lung_nume_fis,fp);
								
										}
										pozitie3=pozitie3+12;
									}
								}
								pozitie2=pozitie2+12;//next
								fseek(fp,pozitie2,0);
								kl--;
							}

						}
					
				}
			}
			if(ok==1){
				printf("SUCCES\n");
			}
			if(ok==0){
				printf("INVALID\n");
			}
		
		}








		strcpy(comanda,"asd");//stopeaza o intrare in +
		strcpy(argument,"asd");





}//inchidem fisierele
fclose(fp);
fclose(fp1);
}








//TASK 3
if(strcmp("3",argv[1])==0){
	FILE *fp2;
	FILE *fp1;
	fp1=fopen("mbr.op","r");
	fp2=fopen("mbr.bin","r+b");


	char comanda[100],argument[100];


	while(1){
		if(feof(fp1)){ 
 			break;
		}


		fscanf(fp1,"%s",comanda);

		
		//DELETE_PARTITION
		if(strcmp("DELETE_PARTITION",comanda)==0){
		int pozitie=4,adresa_partie_curenta,ok=0,i,nr_nou_part,nr_partitii;
		char nume_part[10],nume_partd[10];

		int asf=0;


			fscanf(fp1,"%s",argument);

			fseek(fp2,0,0);
			fread(&nr_partitii,sizeof(int),1,fp2);
			nr_nou_part=nr_partitii-1;
			for(i=0;i<nr_partitii;i++){//parcurgem partiile
				fseek(fp2,pozitie,0);
				fread(&adresa_partie_curenta,sizeof(int),1,fp2);
				while(adresa_partie_curenta==0){//sarim peste spatiile goale din mbr
					pozitie=pozitie+4;
					fseek(fp2,pozitie,0);
					fread(&adresa_partie_curenta,sizeof(int),1,fp2);
				}
				fseek(fp2,adresa_partie_curenta+4,0);
				fread(&nume_part,8,1,fp2);
				nume_part[8]='\0';
				if(strcmp(argument,nume_part)==0){//am gasit partitia
					fseek(fp2,0,0);
					fwrite(&nr_nou_part,1,sizeof(int) ,fp2);//actualizam numarul de partitii
					fseek(fp2,pozitie,0);
					fwrite(&asf,1,sizeof(int),fp2);//bagam 0 peste partitia de sters
					nr_nou_part--;
					ok=1;
					break;
				}
				pozitie=pozitie+4;//next


			}	if(ok==1){

				printf("SUCCES\n");
				}
				if(ok==0){
				
				printf("INVALID\n");
				}
		}
		//FORMAT_PARTITION
		if(strcmp("FORMAT_PARTITION",comanda)==0){
			int i,pozitie=4,ok=0,adresa_partie_curenta,adresa_partie_urmatoare,k,asf=0,stop,nr_partitii;
			char nume_part[10];
			fscanf(fp1,"%s",argument);
			fseek(fp2,0,0);
			fread(&nr_partitii,sizeof(int),1,fp2);
			for(i=0;i<nr_partitii;i++){//parcurgem partiile
				fseek(fp2,pozitie,0);
				fread(&adresa_partie_curenta,sizeof(int),1,fp2);
				while(adresa_partie_curenta==0){//sarim peste spatiile goale din mbr
					pozitie=pozitie+4;
					fseek(fp2,pozitie,0);
					fread(&adresa_partie_curenta,sizeof(int),1,fp2);
				}
				fseek(fp2,adresa_partie_curenta+4,0);
				fread(&nume_part,8,1,fp2);
				nume_part[8]='\0';
				
				if(strcmp(argument,nume_part)==0){//am gasit partitia
				ok=1;
					if(i!=nr_partitii-1){//cat timp nu e ultima
						fseek(fp2,pozitie+4,0);
						fread(&adresa_partie_urmatoare,sizeof(int),1,fp2);

						fseek(fp2,adresa_partie_curenta+20,0);
						k=0;
						while(k<(adresa_partie_urmatoare-adresa_partie_curenta-20)/4){
							fwrite(&asf,1,sizeof(int) ,fp2);//bagam 0 pe toata lungimea ei pana la urmatoarea
							k++;
						}
					break;
					}
					
					if(i==nr_partitii-1){//tratam cazul cu ultima separat
						
						k=0;

						fseek (fp2,0,2);
						stop=ftell(fp2);

						fseek(fp2,adresa_partie_curenta+20,0);
						while(k<(stop-adresa_partie_curenta-20)/4){//bagam zerouri de unde incepe partitia pana la finalul fisierului

							fwrite(&asf,1,sizeof(int) ,fp2);
							k++;
						}
					break;
					}
						
				}
				pozitie=pozitie+4;
			}

				if(ok==0){
					printf("INVALID\n");
				}
				if(ok==1){
					printf("SUCCES\n");
				}
		}

		//CREATE_PARTITION
		if(strcmp("CREATE_PARTITION",comanda)==0){
			int i,pozitie=4,MAXD,nr_partitii,MAXF,spatiu_necesar,ok=1,adresa_partie_curenta,spatiu_part,adresa_part_noua,ok2=0,spatiu_disponibil=0,adresa_partie_ant,adresa_partie_urm,MAXFa,MAXDa;
			char nume_part[10];
			fscanf(fp1,"%s",argument);
			fscanf(fp1,"%d",&MAXD);
			fscanf(fp1,"%d",&MAXF);
			spatiu_necesar=20+MAXD*12+MAXF*12+MAXD*MAXF*12;
			fseek(fp2,0,0);
			fread(&nr_partitii,sizeof(int),1,fp2);
			for(i=0;i<nr_partitii;i++){//parcurgem partitiile
				fseek(fp2,pozitie,0);
				fread(&adresa_partie_curenta,sizeof(int),1,fp2);
				fseek(fp2,adresa_partie_curenta+4,0);
				fread(&nume_part,8,1,fp2);
				nume_part[8]='\0';
				
				if(strcmp(argument,nume_part)==0){//daca deja exista ne oprim
					ok=0;
					break;
				}

				pozitie=pozitie+4;
			}
			if(ok==1){//daca nu exista
				pozitie=4;
			
				while(1){
					fseek(fp2,pozitie,0);
					fread(&spatiu_part,sizeof(int),1,fp2);
					spatiu_disponibil=0;
					if(spatiu_part==8){//in caz ca n-am gasit loc in mbr si am ajuns in prima partitie
						break;//ne oprim
					}
					if(spatiu_part==0){//am gasit loc in mbr
						fseek(fp2,pozitie-4,0);
						fread(&adresa_partie_ant,sizeof(int),1,fp2);
						fseek(fp2,adresa_partie_ant+12,0);
						fread(&MAXDa,sizeof(int),1,fp2);
						fread(&MAXFa,sizeof(int),1,fp2);
						fseek(fp2,pozitie+4,0);
						fread(&adresa_partie_urm,sizeof(int),1,fp2);
						spatiu_disponibil=adresa_partie_urm-adresa_partie_ant-(20+MAXDa*12+MAXFa*12		+MAXDa*MAXFa*12);//calculam spatiul disponibil scazand din partia urmatoare lungimea celei anterioare
						
						if(spatiu_disponibil>=spatiu_necesar){//daca am gasit spatiu
							adresa_part_noua=adresa_partie_ant+20+MAXDa*12+MAXFa*12+MAXDa*MAXFa*12;//calculam adresa de inceput
							ok2=1;
							break;
						}
						else{
							ok2=0;
						}
					}

				pozitie=pozitie+4;
				}
			}
				if(ok==0 || ok2==0){
					printf("INVALID\n");
				}
				if(ok==1 && ok2==1){//daca totul e ok pana aici
					int opt=8;
					fseek(fp2,0,0);
					fread(&nr_partitii,sizeof(int),1,fp2);
					int nr_nou_part=nr_partitii+1;
					fseek(fp2,0,0);
					fwrite(&nr_nou_part,1,sizeof(int),fp2);//actualizam nr de partitii
					fseek(fp2,pozitie,0);
					fwrite(&adresa_part_noua,1,sizeof(int),fp2);//bagam adresa noua
					fseek(fp2,adresa_part_noua,0);
					fwrite(&opt,1,sizeof(int),fp2);//bagam lungimea numelui
					fwrite(&argument,1,8,fp2);//bagam numele
					fwrite(&MAXD,1,sizeof(int),fp2);//bagam maxd 
					fwrite(&MAXF,1,sizeof(int),fp2);//bagam maxf
					printf("%d\n",adresa_part_noua);//afisam adresa partitiei
				}

					






}//create

		strcpy(comanda,"asd");//stopeaza o intrare in +  
		strcpy(argument,"asd");
}//if a 3a
fclose(fp2);//inchidem fisierele
fclose(fp1);
}//while sus



	//BONUS
	if(strcmp("4",argv[1])==0){
		FILE *fpb;//fiser
		fpb=fopen("mbr.bin","rb");
		int MBR_SIZE,pozitie,i=1;
		char c[7],ce[8];


		fseek(fpb,0,0);
		fread(&MBR_SIZE,sizeof(int),1,fpb);
		printf("MBR_SIZE: %d\n",MBR_SIZE);//scotem mbr size
		printf("PARTITIONS_OFFSET: ");
		pozitie=MBR_SIZE*4;

		while(1){
			if(feof(fpb))
		      	{ 
			 break;
		      	}
			fseek(fpb,pozitie+4,0);
			fread(&c,7,1,fpb);
			c[7]='\0';
			if(i!=MBR_SIZE){//cautam fix cate partii sunt
				if(strcmp(c,"/dev/sd")==0){//dupa inceputul numelui lor 
					fseek(fpb,pozitie,0);
					printf("%li ",ftell(fpb));//afisam adresa in caz ca am gasit ceva
					i++;
				}
			}else{
				if(strcmp(c,"/dev/sd")==0){
					fseek(fpb,pozitie,0);
					printf("%li",ftell(fpb));
					i++;
				}
			}
			pozitie=pozitie+4;
				
		}

	}



return 0;
}
