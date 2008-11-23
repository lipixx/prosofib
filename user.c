#include <libc.h>
#include <types.h>
#include <stdio.h>
#include <jocs.h>
#include <sf.h>
#include <utils.h>

void runjp ();


int strlen(char * buffer)
{int i;
	for (i=0; buffer[i] != 0; i++);
	return i;
}

int __attribute__ ((__section__ (".text.main"))) main (void)
{
  printf ("\nBenvinguts a ZeOS!");  
 
char buffer[1024];
	int tam, tam2, fd, pid, fd2, i, num, ret;
	int ok = 0;
	char *frasellarga = "INI45678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567FIN\n";
	struct dir_ent entrada;
	
	sem_init(1,0);
	sem_init(0,0);
  	pid = fork();
	if (pid == 0){
	    for(i=0; i<80*35; i++) write(1," ",1);
	    write(1,"\n",1);
	    while(1){	

		write(1,"Tria el joc de probes que vols correr:\n",39);
		write(1,"1->write.\n",10);
		write(1,"2->read.\n",9);
		write(1,"3->open.\n",9);
		write(1,"4->dup.\n",8);
		write(1,"5->close i unlink.\n",19);
		write(1,"6->readdir.\n",12);
		write(1,"7->herencia de canals.\n",23);
		write(1,"8->entrada per teclat.\n",23);
		write(1,"\n",1);
		tam = read(0, buffer, 1);
		num = buffer[0] - '0';
		switch (num){
			case 1:
				for(i=0; i<80*39; i++) write(1," ",1);
				write(1,"#################WRITE##################\n",41);
				write(1,"Provem els errors del write:\n",29);
				
				fd = open("f1",O_RDWR|O_CREAT);
				fd2 = open("f1",O_RDONLY);
				
				write(1,"Canal incorrecte: ",18); tam = write(1000,"hola",4); if(tam != -1) ok =-1; perror();
				write(1,"Canal buit: ",12); tam = write(7,"hola",4); if(tam != -1) {ok =-1; perror();}
				write(1,"Size negatiu: ",14); tam = write(fd,"hola",-3); if(tam != -1) ok =-1; perror();
				write(1,"@ buffer incorrecte: ",21); tam = write(fd,(char*)1000,4); if(tam != -1) ok =-1; perror();
				write(1,"Escriure en fitxer obert a lectura: ",36); tam = write(fd2,"hola",4); if(tam != -1) ok =-1; perror();

				write(1,"\n",1);
				write(1,"Provem write correctament:\n",27);
				write(1,"Provem write < 256: ",19); tam = write(fd,"Frase curta.\n",13); read(fd2,buffer,tam); tam = write(1,buffer,tam);
				 if(tam != 13) ok =-1;

				close(fd);
				close(fd2);
				unlink("f1");
				fd = open("f1",O_RDWR|O_CREAT);
				fd2 = open("f1",O_RDONLY);
				write(1,"Provem write > 512: ",19); tam = write(fd,frasellarga,531); read(fd2,buffer,tam); tam = write(1,buffer,tam);
				if(tam != 531) ok =-1;
		
				write(1,"\n",1);
				if (ok == -1) write(1,"HA FALLAT ALGUNA COSA.\n",24);
				else write(1,"TOT HA ANAT BE.\n",16);

				write(1,"\n",1);
				write(1,"Apreta qualsevol tecla per tornar a la pantalla principal.\n",59);
				read(0,buffer,1);
				for(i=0; i<80*30; i++) write(1," ",1);
				write(1,"\n",1);

				close(fd);
				close(fd2);
				unlink("f1");
				break;

			case 2:
				for(i=0; i<80*39; i++) write(1," ",1);
				write(1,"#################READ##################\n",40);
				write(1,"Provem els errors del read:\n",28);
				
				fd = open("f1",O_RDWR|O_CREAT);
				fd2 = open("f1",O_WRONLY);
				
				write(1,"Canal incorrecte: ",18); tam = read(1000,buffer,4); if(tam != -1) ok =-1; perror();
				write(1,"Canal buit: ",12); tam = read(7,buffer,4); if(tam != -1) ok =-1; perror();
				write(1,"Size negatiu: ",14); tam = read(fd,buffer,-3); if(tam != -1) ok =-1; perror();
				write(1,"@ buffer incorrecte: ",21); tam = read(fd,(char*)1000,4); if(tam != -1) ok =-1; perror();
				write(1,"Llegir en fitxer obert a escriptura: ",37); tam = read(fd2,"hola",4); if(tam != -1) ok =-1; perror();

				write(1,"\n",1);
				write(1,"Provem read correctament:\n",26);
				write(fd,frasellarga,531);
				close(fd2);
				fd2 = open("f1",O_RDONLY);

				write(1,"Provem read < 256: ",19); tam = read(fd2,buffer,10); write(1,buffer,tam); write(1,"\n",1);
				if(tam != 10) ok =-1;
				write(1,"Provem read > 512: ",19); tam = read(fd2,buffer,521); write(1,buffer,tam);
				if(tam != 521) ok =-1;
		
				write(1,"\n",1);
				if (ok == -1) write(1,"HA FALLAT ALGUNA COSA.\n",24);
				else write(1,"TOT HA ANAT BE.\n",16);

				write(1,"\n",1);
				write(1,"Apreta qualsevol tecla per tornar a la pantalla principal.\n",59);
				read(0,buffer,1);
				for(i=0; i<80*29; i++) write(1," ",1);
				write(1,"\n",1);

				close(fd);
				close(fd2);
				unlink("f1");
				break;
			case 3:
				for(i=0; i<80*39; i++) write(1," ",1);
				write(1,"#################OPEN##################\n",40);
				write(1,"Provem els errors del open:\n",28);
				
				write(1,"Nom buit: ",10); fd = open("",O_RDWR|O_CREAT); if(fd != -1) ok =-1; perror();
				write(1,"Tamany del nom superior a vuit: ",32); fd = open("1234567890",O_RDWR|O_CREAT); if(fd != -1) ok =-1; perror();
				write(1,"Flags inexistents: ",19); fd = open("f1",777); if(fd != -1) ok =-1; perror();
				write(1,"Fem un open d'escriptura d'un dispositiu nomes de lectura: ",59); fd = open("keyboard",O_WRONLY); if(fd != -1) ok =-1; perror();
				write(1,"Fem un open de lectura d'un dispositiu nomes d'escriptura: ",59); fd = open("screen",O_RDONLY); if(fd != -1) ok =-1; perror();

				for(i=0;i<8;i++) dup(0);
				write(1,"Taula de canals plena: ",23); fd = open("f1",O_RDWR|O_CREAT); if(fd != -1) ok =-1; perror();
				for(i=0;i<8;i++) close(i+2);

				open("screen",O_WRONLY);open("screen",O_WRONLY);open("screen",O_WRONLY);open("screen",O_WRONLY);open("screen",O_WRONLY);open("screen",O_WRONLY);open("screen",O_WRONLY);open("screen",O_WRONLY);
				write(1,"TFO plena: ",11); fd = open("f1",O_RDWR|O_CREAT); if(fd != -1) ok =-1; perror();
				for(i=0;i<8;i++) close(i+2);

				open("1",O_RDWR|O_CREAT);open("2",O_RDWR|O_CREAT);open("3",O_RDWR|O_CREAT);open("4",O_RDWR|O_CREAT);open("5",O_RDWR|O_CREAT);open("6",O_RDWR|O_CREAT);open("7",O_RDWR|O_CREAT);open("8",O_RDWR|O_CREAT);
				write(1,"Directori ple: ",15); fd = open("f1",O_RDWR|O_CREAT); if(fd != -1) ok =-1; perror();
				for(i=0;i<8;i++) close(i+2); unlink("1");unlink("2");unlink("3");unlink("4");unlink("5");unlink("6");unlink("7");unlink("8");

				
				write(1,"\n",1);
				write(1,"Provem open correctament:\n",26);
				write(1,"Creem dos fitxer nous: ",23);
					fd = open("f1",O_RDWR|O_CREAT); if(fd != 2) ok =-1;
					fd = open("f2",O_RDWR|O_CREAT); if(fd != 3) ok =-1;
					readdir(&entrada, 2); write(1,"[",1);write(1,entrada.nom,2);write(1,"] ",2);
					readdir(&entrada, 3); write(1,"[",1);write(1,entrada.nom,2);write(1,"]\n",2);

				write(1,"Fem un segon open d'un fitxer ja creat: ",40); fd = open("screen",O_WRONLY); if(fd != 4) ok =-1;
					write(fd, "Si surt aixo, esta correcte.\n",29);

				write(1,"Creem un fitxer ja creat.\n",26); fd = open("f1",O_RDWR|O_CREAT); if(fd != 5) ok =-1;

				write(1,"Imprimim tot el directori: ",27);
					for(i=0;i<4;i++){
						readdir(&entrada, i);
						write(1,"[",1);write(1,entrada.nom,10);write(1,"] ",2);
					}
					write(1,"\n",1);

				//Tanquem i eliminem els fitxers
				for(i=2;i<6;i++){
					close(i);
				}
				unlink("f1");unlink("f2");

				write(1,"\n",1);
				if (ok == -1) write(1,"HA FALLAT ALGUNA COSA.\n",24);
				else write(1,"TOT HA ANAT BE.\n",16);

				write(1,"\n",1);
				write(1,"Apreta qualsevol tecla per tornar a la pantalla principal.\n",59);
				read(0,buffer,1);
				for(i=0; i<80*30; i++) write(1," ",1);
				write(1,"\n",1);
				break;
			case 4:
				for(i=0; i<80*39; i++) write(1," ",1);
				write(1,"#################DUP##################\n",39);
				write(1,"Provem els errors del dup:\n",27);
				
				write(1,"Canal incorrecte: ",18); fd = dup(1000); if(fd != -1) ok =-1; perror();
				write(1,"Canal buit: ",12); fd = dup(7); if(fd != -1) ok =-1; perror();				

				write(1,"\n",1);
				write(1,"Provem dup correctament:\n",25);
				write(1,"Dupliquem canal de pantalla: ",29); 
					fd=dup(1); write(fd,"Si surt aixo es que ha anat be.\n",32); if(fd != 2) ok =-1;
				write(1,"Dupliquem canal de teclat. Escriu una paraula de 4 lletres: ",60);
					fd=dup(0); read(fd,buffer,4); write(1,buffer,4); if(fd != 3) ok =-1;
				write(1,"\n",1);
				write(1,"Creem un fitxer nou, fem un dup i escrivim i llegim hola: ",58); 
					fd = open("f1",O_RDWR|O_CREAT); if(fd != 4) ok =-1;
					fd= dup(fd); if(fd != 5) ok =-1;
					fd2 = open("f1",O_RDONLY); if(fd2 != 6) ok =-1;
					tam = write(fd, "hola\n",5);
					read(fd2,buffer,tam);
					write(1,buffer,tam);
				write(1,"Utilitzem el dup per escriure per pantalla un fitxer a trossos:\n",64);
					fd = open("f2",O_RDWR|O_CREAT); if(fd != 7) ok =-1;
					fd2 = open("f2",O_RDONLY); if(fd2 != 8) ok =-1;
					tam = write(fd,frasellarga,531);
					fd = dup(fd2); if(fd != 9) ok =-1;
					write(1,"Primer tros: ",13);
						tam2 = read(fd,buffer,215);
						write(1,buffer,tam2);
						tam -= tam2;
						write(1,"\n",1);
					write(1,"Segon tros: ",12);
						tam2 = read(fd2,buffer,198);
						write(1,buffer,tam2);
						tam -= tam2;
						write(1,"\n",1);
					write(1,"Tercer tros: ",13);
						tam2 = read(fd,buffer,tam);
						write(1,buffer,tam2);				
					

				//Tanquem els canals i eliminem els fitxers
				for(i=2;i<10;i++){
					close(i);
				}
				unlink("f1");unlink("f2");

				write(1,"\n",1);
				if (ok == -1) write(1,"HA FALLAT ALGUNA COSA.\n",24);
				else write(1,"TOT HA ANAT BE.\n",16);

				write(1,"\n",1);
				write(1,"Apreta qualsevol tecla per tornar a la pantalla principal.\n",59);
				read(0,buffer,1);
				for(i=0; i<80*28; i++) write(1," ",1);
				write(1,"\n",1);
				break;

			case 5:
				for(i=0; i<80*39; i++) write(1," ",1);
				write(1,"#################CLOSE & UNLINK##################\n",50);
				write(1,"Provem els errors del close:\n",29);
				
				write(1,"Canal incorrecte: ",18); fd = close(1000); if(fd != -1) ok =-1; perror();
				write(1,"Canal buit: ",12); fd = close(7); if(fd != -1) ok =-1; perror();				

				write(1,"\n",1);
				write(1,"Provem els errors del unlink:\n",30);

				write(1,"Nom buit: ",10); fd = unlink(""); if(fd != -1) ok =-1; perror();
				write(1,"Tamany del nom superior a vuit: ",32); fd = unlink("1234567890"); if(fd != -1) ok =-1; perror();
				write(1,"Unlink de fitxer inxistent: ",28); fd = unlink("hola"); if(fd != -1) ok =-1; perror();
				write(1,"Unlink de fitxer obert: ", 24); 
					fd = open("f1",O_RDWR|O_CREAT); if(fd != 2) ok =-1; perror();
					fd = unlink("f1"); if(fd != -1) ok =-1; perror();
					close(2);
					unlink("f1");

				write(1,"\n",1);
				write(1,"Provem close i unlink correctament:\n",36);
				write(1,"Creem un fitxer f1.\n",20); fd = open("f1",O_RDWR|O_CREAT); if(fd != 2) ok =-1;
				write(1,"Printem tot el directori: ",26);
					for(i=0;i<10;i++){
						if (readdir(&entrada, i) == 0) {write(1,"[",1);write(1,entrada.nom,10);write(1,"] ",2);}
					}
					write(1,"\n",1);
				write(1,"Fem un close de f1.\n",20); fd = close(fd); if(fd != 0) ok =-1;
				write(1,"Fem un unlink de f1.\n",21); fd = unlink("f1"); if(fd != 0) ok =-1;
				write(1,"Printem tot el directori: ",26);
					for(i=0;i<10;i++){
						if (readdir(&entrada, i) == 0) {write(1,"[",1);write(1,entrada.nom,10);write(1,"] ",2);}
					}
					write(1,"\n",1);

				write(1,"\n",1);
				if (ok == -1) write(1,"HA FALLAT ALGUNA COSA.\n",24);
				else write(1,"TOT HA ANAT BE.\n",16);

				write(1,"\n",1);
				write(1,"Apreta qualsevol tecla per tornar a la pantalla principal.\n",59);
				read(0,buffer,1);
				for(i=0; i<80*28; i++) write(1," ",1);
				write(1,"\n",1);
				break;

			case 6:
				for(i=0; i<80*39; i++) write(1," ",1);
				write(1,"#################READDIR##################\n",43);
				write(1,"Provem els errors del readdir:\n",31);
				
				write(1,"Offset incorrecte: ",19); fd = readdir(&entrada,-1); if(fd != -1) ok =-1; perror();
				write(1,"Offset d'un directori buit: ",28); readdir(&entrada,7); if(fd != -1) ok =-1; perror();	
				write(1,"@dir_ent incorrecte: ",21); readdir(1000,1); if(fd != -1) ok =-1; perror();	


				write(1,"\n",1);
				write(1,"Provem readdir correctament:\n",29);
			
				fd = open("f1",O_RDWR|O_CREAT);
				fd = open("f2",O_RDWR|O_CREAT);
				fd = open("f3",O_RDWR|O_CREAT);
				fd = open("f4",O_RDWR|O_CREAT);
				fd = open("f5",O_RDWR|O_CREAT);
				write(1,"Creem 5 fitxers.\n",17);
				write(1,"Printem tot el directori: ",26);
					for(i=0;i<10;i++){
						if (readdir(&entrada, i) == 0) {
							write(1,"[",1);
							write(1,entrada.nom,10);
							write(1,", ",2);
							itoa(entrada.size, buffer);
							write(1,buffer,strlen(buffer));
							write(1,"] ",2);}
					}
					write(1,"\n",1);
				write(1,"Fem un close de f1.\n",20); fd = close(2); if(fd != 0) ok =-1;
				write(1,"Fem un unlink de f1.\n",21); fd = unlink("f1"); if(fd != 0) ok =-1;
				write(1,"Fem un write de tamany 531 a f2.\n",33); write(3,frasellarga,531);
				write(1,"Creem el fitxer f6.\n",20);fd = open("f6",O_RDWR|O_CREAT);
				write(1,"Printem tot el directori: ",26);
					for(i=0;i<10;i++){
						if (readdir(&entrada, i) == 0) {
							write(1,"[",1);
							write(1,entrada.nom,10);
							write(1,", ",2);
							itoa(entrada.size, buffer);
							write(1,buffer,strlen(buffer));
							write(1,"] ",2);}
					}
					write(1,"\n",1);

				write(1,"Eliminem tots els fitxers creats.\n",34); 
				//Tanquem tots els canals
				for(i=2;i<10;i++){
					close(i);
				}
				unlink("f2"); unlink("f3"); unlink("f4"); unlink("f5"); unlink("f6");
				write(1,"Printem tot el directori: ",26);
					for(i=0;i<10;i++){
						if (readdir(&entrada, i) == 0) {
							write(1,"[",1);
							write(1,entrada.nom,10);
							write(1,", ",2);
							itoa(entrada.size, buffer);
							write(1,buffer,strlen(buffer));
							write(1,"] ",2);}
					}
					write(1,"\n",1);

				write(1,"\n",1);
				if (ok == -1) write(1,"HA FALLAT ALGUNA COSA.\n",24);
				else write(1,"TOT HA ANAT BE.\n",16);

				write(1,"\n",1);
				write(1,"Apreta qualsevol tecla per tornar a la pantalla principal.\n",59);
				read(0,buffer,1);
				for(i=0; i<80*28; i++) write(1," ",1);
				write(1,"\n",1);
				break;
			case 7:
				for(i=0; i<80*39; i++) write(1," ",1);
				write(1,"#################HERENCIA DE CANALS##################\n",54);
				write(1,"Aqui demostrem que els canals s'herencien correctament.\n",56);
				write(1,"\n",1);

				write(1,"Creem un fitxer f1.\n",20); fd = open("f1",O_RDWR|O_CREAT); if(fd != 2) ok =-1;
				write(1,"Fem un fork.\n",13);
				pid = fork();
				if (pid == 0){
					sem_wait(0);
					write(1,"Soc el proces ",14);
					itoa(getpid(),buffer);
					write(1,buffer,strlen(buffer));
					write(1," i escric a f1.\n",16); write(fd, frasellarga, 531);
					write(1,"Soc el proces ",14);
					itoa(getpid(),buffer);
					write(1,buffer,strlen(buffer));
					write(1," i faig el close de fd.\n",24); ret = close(fd); if (ret != 0) ok = -1;
					sem_signal(1);
					exit();
				}
				write(1,"Soc el proces ",14);
				itoa(getpid(),buffer);
				write(1,buffer,strlen(buffer));
				write(1," i faig el close i intento fer l'unlink abans que el proces ", 60);
				itoa(pid,buffer);
				write(1,buffer,strlen(buffer));
				write(1," faci el seu close: ",20);
					ret = close(fd); if (ret != 0) ok = -1;
					ret = unlink("f1"); if (ret != -1) ok = -1; perror();
				
				sem_signal(0);
				sem_wait(1);

				write(1,"Soc el proces ",14);
				itoa(getpid(),buffer);
				write(1,buffer,strlen(buffer));
				write(1," i escric per pantalla el que hi ha en f1: ", 43);
					fd2 = open("f1", O_RDONLY);
					tam = read(fd2,buffer,531);
					write(1,buffer,tam);
					close(fd2);

				write(1,"Soc el proces ",14);
				itoa(getpid(),buffer);
				write(1,buffer,strlen(buffer));
				write(1," i ara si que faig l'unlink de f1.\n", 35);
				ret = unlink("f1"); if (ret != 0) ok = -1;
				
				write(1,"\n",1);
				if (ok == -1) write(1,"HA FALLAT ALGUNA COSA.\n",24);
				else write(1,"TOT HA ANAT BE.\n",16);

				write(1,"\n",1);
				write(1,"Apreta qualsevol tecla per tornar a la pantalla principal.\n",59);
				read(0,buffer,1);
				for(i=0; i<80*28; i++) write(1," ",1);
				write(1,"\n",1);				
				break;

			case 8:
				for(i=0; i<80*39; i++) write(1," ",1);
				write(1,"#################LLEGIR DE TECLAT##################\n",52);
				
				write(1,"Escriu una frase de 10 lletres: ",32);
					tam = read(0,buffer,10);
					write(1,buffer,tam);
				write(1,"\n",1);
				write(1,"\n",1);

				write(1,"Escriu una frase de 50 lletres: ",32);
					tam = read(0,buffer,50);
					write(1,buffer,tam);
				write(1,"\n",1);
	
				write(1,"\n",1);
				write(1,"Apreta qualsevol tecla per tornar a la pantalla principal.\n",59);
				read(0,buffer,1);
				for(i=0; i<80*43; i++) write(1," ",1);
				write(1,"\n",1);				
				break;

			default:
				for(i=0; i<80*39; i++) write(1," ",1);
				write(1,"Escriu un numero entre el 1 i el 7.\n",36);
				break;
		}
				
		
	    }
	exit();
	}
	nice(1);
  	while(1);

  /* __asm__ __volatile__ ("movl $40,%%eax\n"
			"int $0x80\n"
			:
			:
			);
  */
  //provar_fork (9); //No sobrepassar NR_TASKS

  //provar_get_stats ();

  //provar_exit ();

  //provar_nice ();

  //provar_switch ();

  //provar_semafors ();

  /* JOCS DE PROVES DE LA PRIMERA ENTREGA */

  //runjp (); //Hi ha que posar NUM_PAG_DATA = 8

  // int fd = open("patata",O_WRONLY|O_CREAT);

  //run2_jp();
  /*
  struct dir_ent buffer;
  int fd,fd2,i;
  
  fd = open ("patata",O_RDWR|O_CREAT);
  fd2 = open ("cuminu",O_RDWR|O_CREAT);

  //close(fd);

  unlink("patata");

  perror();
  */
//  while (1);
 // return 0;
}

