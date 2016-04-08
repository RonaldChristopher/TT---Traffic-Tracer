#include <stdio.h>
#include <stdlib.h>
//#include <pcap.h>
//#include <errno.h>
#include <string.h>
//#include <map>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
#include <math.h>
#include <time.h>

long int pktcount;
long long int total_bits;
int window = 55;
int val = 10000;
/* Read the packet file */
int packet_count(char *in_fn)
{
    srand ( time(NULL) );
    int         portnum,  count = 0, Src, Dst, Srcport, Dstport, Load;
    float time;
    FILE        *fp;

    fp = fopen(in_fn, "r");
    if( fp == NULL ){
        fprintf(stderr, "Cannot read routing table file %s.\n", in_fn);
        exit(1);
    }

    while( fscanf(fp, "%f %d %d %d %d %d", &time, &Src, &Dst, &Srcport, &Dstport, &Load) != EOF )
    {
        count++;
    }
    return count;
}

double pktprocess(char *in_fn,float src[], float dst[],float srcport[],float dstport[],float load[],float time[])
{

        double  totaltime= 0, bitrate = 0;
	long long int totalbits=0;
	FILE        *fp;
        fp = fopen(in_fn, "r");

      if( fp == NULL )
      {
        fprintf(stderr, "Cannot read routing table file %s.\n", in_fn);
        exit(1);
      }
      for( int i = 0; i< pktcount; i++)
      {
     	 fscanf(fp, "%f %f %f %f %f %f", &time[i], &src[i], &dst[i], &srcport[i], &dstport[i], &load[i]);
	 totalbits = totalbits + load[i]*8.0 + (40*8);
      }

      totaltime = time[pktcount-1] - time[0];
      bitrate = totalbits / totaltime;
      total_bits = totalbits;
      return bitrate;
}

/* main function */
int main(int argc, char **argv)
{

        pktcount = packet_count(argv[1]);
	int temp = 0, temp1 = 0, k =0, y=0, cnt=0, l=0, max = 4, const_val =0, i4=0,number = 0, s, des, sp, dp;;
	long int a=0, b=0, c=0, at=0, d=0, count_val =0;
	float tem =0.0;
	double maxdiff = 0.0;
	long int i=0, totalbits=0 ;
        float tottime=0, totaltime=0, j = 0.0, total=0, count[6],count1[6];
        double  bitrate1=0, newpktcnt=0.0;
	float portsort[4];
	float bitrate[65];
	long long int* counter;
	long int* counter1;
	int* srcval;
	int* srcval1;
	float* num;
	float* num1;
	float* src;
        float* dst;
	float* srcport;
	float* dstport;
	float* load;
	float* time;
	float* port1;
	float* port2;
	float* port3;
	float* port4;
	float* time1;
	float* time2;
	float* time3;
	float* time4;
        src = (float*)malloc(pktcount*sizeof(float));
	dst = (float*)malloc(pktcount*sizeof(float));
	srcport = (float*)malloc(pktcount*sizeof(float));
	dstport = (float*)malloc(pktcount*sizeof(float));
	load = (float*)malloc(pktcount*sizeof(float));
        time = (float*)malloc(pktcount*sizeof(float));
	counter = (long long int*)malloc(pktcount*sizeof(long long int));
	counter1 = (long int*)malloc(pktcount*sizeof(long int));
	srcval = (int*)malloc(pktcount*sizeof(int));
	srcval1 = (int*)malloc(pktcount*sizeof(int));
  	num = (float*)malloc(pktcount*sizeof(float));
	num1 = (float*)malloc(pktcount*sizeof(float));
	port1 = (float*)malloc((pktcount)*sizeof(float));
	port2 = (float*)malloc((pktcount)*sizeof(float));
	port3 = (float*)malloc((pktcount)*sizeof(float));
	port4 = (float*)malloc((pktcount)*sizeof(float));
	time1 = (float*)malloc((pktcount)*sizeof(float));
	time2 = (float*)malloc((pktcount)*sizeof(float));
	time3 =	(float*)malloc((pktcount)*sizeof(float));
	time4 = (float*)malloc((pktcount)*sizeof(float));

	float buffsrc[window], bufftime[window], buffdst[window], buffsrcport[window], buffdstport[window], buffload[window];


        fprintf(stderr, "\nDone with packet processing! Looked up %ld packets.\n", pktcount);

	// 1. To find the bit rate of the packets.
        bitrate1 = pktprocess(argv[1], src, dst, srcport, dstport, load, time);
        printf("\nAverage bit rate of the traffic trace file= %lf Kbps \n", (bitrate1/1024));

	// 2. To find the bit rate of the packets in 5 min intervals.
        printf("\nThe Bit-rate in the intervals of five minutes:\n\n");
        printf("Window\t\tAverage Bit Rate(Kbps)\n");

	tottime = time[pktcount-1] - time[0];
	tottime = tottime/60;
	y=5;
	j = fmod(tottime,5.0);
	tottime = tottime + (5.0 - j);
	j=0.0;

	for (i= 0; i< 65;i++)
		bitrate[i] =0;

	for( j = 5.0; j<= tottime; j=j+5.0)
	{
		for( i = temp; i< pktcount; i++)
		{
			if((time[i]/60.0) < j)
			{
				totalbits = totalbits + load[i]*8.0 + (40*8);
				temp1 ++;

			}


		}
		temp+= temp1;
		totaltime = time[temp-1] - time[temp-temp1];
        	bitrate[k] = totalbits / totaltime;
		totalbits = 0;
	        totaltime = 0.0;
		printf("%d-%d\t\t%f\n",y-5,y , bitrate[k]/1024.0);
		temp1 = 0;
		k++;
		y+=5;
	}

	 // 3. To find the distribution of packets based on their payload sizes.

	for (i = 0;i<6;i++)
	{
		count[i] =0;
		count1[i]=0;
	}

	for( i = 0; i< pktcount; i++)
	{
		if(load[i]==0)
			count[0]++;
		else if (1<= load[i] && load[i]<=127)
			 count[1]++;

		else if(128<= load[i] && load[i]<=255)
			count[2]++;

		else if(256<= load[i] && load[i]<=383)
			count[3]++;

		else if(384<= load[i] && load[i]<=511)
			count[4]++;

	        else if (load[i] == 512)
			count[5]++;
	}

	newpktcnt = pktcount;
	printf("\nPayload Size\t\tPercentage based on number of packets\n");
	count1[0] = ((count[0]/newpktcnt)*100.0);
	printf("\n0\t\t\t%f", count1[0]);
	count1[1] = ((count[1]/newpktcnt)*100.0);
	printf("\n1-127\t\t\t%f", count1[1]);
	count1[2] = ((count[2]/newpktcnt)*100.0);
	printf("\n128-255\t\t\t%f", count1[2]);
	count1[3] = ((count[3]/newpktcnt)*100.0);
	printf("\n256-383\t\t\t%f", count1[3]);
	count1[4] = ((count[4]/newpktcnt)*100.0);
	printf("\n384-511\t\t\t%f", count1[4]);
	count1[5] = ((count[5]/newpktcnt)*100.0);
	printf("\n512\t\t\t%f\n", count1[5]);


//4. To sort the IP addresses and find out their load

a =0;
const_val = val;
b = 0;
c = 3;
d = 0;

for(i4 = 0; i4 < 215; i4++)
{


		for(i=val-2;i>a;i--)
		{
			for(k=a;k<=i;k++)
			{
				if(src[k] > src[k+1])
				{
					tem = src[k];
					src[k] = src[k+1];
					src[k+1] = tem;

					tem = load[k];
					load[k] = load[k+1];
					load[k+1] = tem;

					tem = dstport[k];
					dstport[k] = dstport[k+1];
					dstport[k+1] = tem;

				}
			}
		}

			temp=a;
			temp1=a;

		k=0;
		cnt = 0;
		i = 0;
		for(i=0;i<const_val;i++)
		{
			counter[i]=0;
			srcval[i]=0;
			num[i]=0;
		}

		do
		{
			counter[temp1] = load[temp];

			for (k=temp+1;k<=val;k++)
			{
				if(src[temp] == src[k])
				{
				   counter[temp1] = counter[temp1] + load[k] ;
				   srcval[temp1] = src[temp];
				   num[temp1]++;
				   cnt++;
				}
			}

			temp = temp+ cnt +1;

			cnt =0;
			temp1++;

		}while(temp<val);


		for(i=a;i<val;i++)
		{
			num[i]++;
		}

		for(i=val-2;i>a;i--)
		{
			for(k=a;k<=i;k++)
			{
				if(counter[k] < counter[k+1])
				{
					tem = counter[k];
					counter[k] = counter[k+1];
					counter[k+1] = tem;

					tem = srcval[k];
					srcval[k] = srcval[k+1];
					srcval[k+1] = tem;

					tem = num[k];
					num[k] = num[k+1];
					num[k+1] = tem;


				}
			}
		}


	for (i = b; i<c; i++)
	{
		counter1[i] = counter[d];
		srcval1[i] = srcval[d];
		num1[i] = num[d];
		count_val++;
		d++;

	}

	d = d + const_val - 3;
	b = c;
	c = c + 3;
	a = val;
	val = val + const_val ;
}

	for(i=count_val-2;i>0;i--)
	{
		for(k=0;k<=i;k++)
		{
			if(srcval1[k] > srcval1[k+1])
			{
				tem = srcval1[k];
				srcval1[k] = srcval1[k+1];
				srcval1[k+1] = tem;

				tem = counter1[k];
				counter1[k] = counter1[k+1];
				counter1[k+1] = tem;


				tem = num1[k];
				num1[k] = num1[k+1];
				num1[k+1] = tem;

			}
		}
	}

	temp=0;
	temp1=0;
	k=0;
	cnt = 0;
	i=0;
	for(i=0;i<count_val;i++)
	{
		counter[i]=0;
		srcval[i]=0;
		num[i]=0;
	}


	i=temp;

	do
	{
		counter[temp1] = counter1[temp];
		num[temp1] = num1[temp];
		for (k=temp+1;k<=count_val;k++)
		{
			if(srcval1[temp] == srcval1[k])
			{
			   counter[temp1] =  counter[temp1] + counter1[k] ;
			   srcval[temp1] = srcval1[temp];
			   num[temp1] = num[temp1] + num1[k]  ;
			   cnt++;
			}

		}

		temp = temp+ cnt +1;

		cnt =0;
		temp1++;

	}while(temp<count_val);

	for(i=count_val-2;i>0;i--)
	{
		for(k=0;k<=i;k++)
		{
			if(counter[k] < counter[k+1])
			{
				tem = counter[k];
				counter[k] = counter[k+1];
				counter[k+1] = tem;

				tem = srcval[k];
				srcval[k] = srcval[k+1];
				srcval[k+1] = tem;

				tem = num[k];
				num[k] = num[k+1];
				num[k+1] = tem;


			}
		}
	}

	printf("\nSource IP\t Traffic (bytes)\t Traffic Percentage\n");

	for (i=0; i<3;i++)
	{
		if(counter[i]!=0)

			printf("\n  %d\t\t   %llu   \t\t  %f", srcval[i],counter[i],((num[i]*40*8 + counter[i]*8)/total_bits)*100.0);

	}
	printf("\n");

	//5. Sort the destination ports according to their traffic volume, list the top 3 destination ports and the corresponding traffic volume and the percentage in the total traffic volume.

	bitrate1 = pktprocess(argv[1], src, dst, srcport, dstport, load, time);


a =0;
val = const_val;
b = 0;
c = 3;
d = 0;

for(i4 = 0; i4 < 215; i4++)
{


		for(i=val-2;i>a;i--)
		{
			for(k=a;k<=i;k++)
			{
				if(dstport[k] > dstport[k+1])
				{
					tem = dstport[k];
					dstport[k] = dstport[k+1];
					dstport[k+1] = tem;

					tem = load[k];
					load[k] = load[k+1];
					load[k+1] = tem;
				}
			}
		}

			temp=a;
			temp1=a;

		k=0;
		cnt = 0;
		i = 0;
		for(i=0;i<const_val;i++)
		{
			counter[i]=0;
			srcval[i]=0;
			num[i]=0;
		}

		do
		{
			counter[temp1] = load[temp];

			for (k=temp+1;k<=val;k++)
			{
				if(dstport[temp] == dstport[k])
				{
				   counter[temp1] = counter[temp1] + load[k] ;
				   srcval[temp1] = dstport[temp];
				   num[temp1]++;
				   cnt++;
				}
			}

			temp = temp+ cnt +1;

			cnt =0;
			temp1++;

		}while(temp<val);


		for(i=a;i<val;i++)
		{
			num[i]++;
		}

		for(i=val-2;i>a;i--)
		{
			for(k=a;k<=i;k++)
			{
				if(counter[k] < counter[k+1])
				{
					tem = counter[k];
					counter[k] = counter[k+1];
					counter[k+1] = tem;

					tem = srcval[k];
					srcval[k] = srcval[k+1];
					srcval[k+1] = tem;

					tem = num[k];
					num[k] = num[k+1];
					num[k+1] = tem;


				}
			}
		}


	for (i = b; i<c; i++)
	{
		counter1[i] = counter[d];
		srcval1[i] = srcval[d];
		num1[i] = num[d];
		count_val++;
		d++;

	}

	d = d + const_val - 3;
	b = c;
	c = c + 3;
	a = val;
	val = val + const_val ;
}




	for(i=count_val-2;i>0;i--)
	{
		for(k=0;k<=i;k++)
		{
			if(srcval1[k] > srcval1[k+1])
			{
				tem = srcval1[k];
				srcval1[k] = srcval1[k+1];
				srcval1[k+1] = tem;

				tem = counter1[k];
				counter1[k] = counter1[k+1];
				counter1[k+1] = tem;


				tem = num1[k];
				num1[k] = num1[k+1];
				num1[k+1] = tem;

			}
		}
	}

	temp=0;
	temp1=0;
	k=0;
	cnt = 0;
	i=0;
	for(i=0;i<count_val;i++)
	{
		counter[i]=0;
		srcval[i]=0;
		num[i]=0;
	}


	i=temp;

	do
	{
		counter[temp1] = counter1[temp];
		num[temp1] = num1[temp];
		for (k=temp+1;k<=count_val;k++)
		{
			if(srcval1[temp] == srcval1[k])
			{
			   counter[temp1] =  counter[temp1] + counter1[k] ;
			   srcval[temp1] = srcval1[temp];
			   num[temp1] = num[temp1] + num1[k]  ;
			   cnt++;
			}

		}

		temp = temp+ cnt +1;

		cnt =0;
		temp1++;

	}while(temp<count_val);

	for(i=count_val-2;i>0;i--)
	{
		for(k=0;k<=i;k++)
		{
			if(counter[k] < counter[k+1])
			{
				tem = counter[k];
				counter[k] = counter[k+1];
				counter[k+1] = tem;

				tem = srcval[k];
				srcval[k] = srcval[k+1];
				srcval[k+1] = tem;

				tem = num[k];
				num[k] = num[k+1];
				num[k+1] = tem;


			}
		}
	}

	printf("\nDestination Port\t Traffic (bytes)\t Traffic Percentage\n");
	newpktcnt = total_bits;
	for (i=0; i<3;i++)
	{
		if(counter[i]!=0)

			printf("\n  %d\t\t\t   %llu  \t\t\t  %f", srcval[i],counter[i],((num[i]*40*8 + counter[i]*8)/newpktcnt)*100.0);

	}
	printf("\n");



	// 6. Load balancing using the round robin fashion.

	bitrate1 = pktprocess(argv[1], src, dst, srcport, dstport, load, time);
	i=0;

        printf("\nThe Bit-rate after splitting into 4 ports in intervals of five minutes:\n\n");
        printf("Window\t\tPort1(Kbps)\t\tPort2(kbps)\t\tPort3(Kbps)\t\tPort4(Kbps)\t\tMax Diff\n");

	tottime = time[pktcount-1] - time[0];
	tottime = tottime/60;
	j = fmod(tottime,5.0);
	tottime = tottime + (5.0 - j);

	j=0.0;
	temp1 =0;
	temp = 0;
	for (i=0; i<4; i++)
		portsort[i]=0;
	i=0;
	y=5;
	a=0;
	for( j = 5.0; j<= tottime; j=j+5.0)
	{
		i = temp;
		while (i<pktcount)
		//for( i = temp; i< pktcount;i++)
		{
			if((time[i]/60.0) < j )
			{


				port1[a] = port1[a] + load[i]*8 + (40*8);
				i++;
				temp1++;
				port2[a] = port2[a] + load[i]*8 + (40*8);
				i++;
				temp1 ++;
				port3[a] = port3[a] + load[i]*8 + (40*8);
				i++;
				temp1++;
				port4[a] = port4[a] + load[i]*8 + (40*8);
				i++;
				temp1++;

			}
			else
				break;


		}

		temp+= temp1;
		totaltime = time[temp-1] - time[temp-temp1];
        	port1[a] = port1[a] /(totaltime*1024);
		port2[a] = port2[a] / (totaltime*1024);
		port3[a] = port3[a] / (totaltime*1024);
		port4[a] = port4[a] / (totaltime*1024);
		temp1 = 0;
		maxdiff =0;
		portsort[0] = port1[a];
		portsort[1] = port2[a];
		portsort[2] = port3[a];
		portsort[3] = port4[a];

		for(l=0;l<4;l++)
		{
			int index_of_min = l;
			for(k=l;k<4;k++)
			{
				if(portsort[index_of_min] < portsort[k])
				{
					index_of_min = k;
				}
			}
			tem = portsort[l];
			portsort[l] = portsort[index_of_min];
			portsort[index_of_min] = tem;
		}
		maxdiff = portsort[0] - portsort[3];
		printf("%d-%d\t\t%f\t\t%f\t\t%f\t\t%f\t\t%f\n",y-5,y , port1[a],port2[a], port3[a], port4[a], maxdiff);
		for(l=0;l<4;l++)
			portsort[l]=0;
		a++;
		y+=5;
	}

	// 7. Algorithm for load balancing using Hashing:

   int i1=0, i2=0, i3=0;



bitrate1 = pktprocess(argv[1], src, dst, srcport, dstport, load, time);
printf("\nThe Bit-rate of New Algorithm using HASHING & HASH Table Concepts in intervals of five minutes:\n\n");
printf("Window\t\tPort1(Kbps)\t\tPort2(kbps)\t\tPort3(Kbps)\t\tPort4(Kbps)\t\tMax Diff\n");

tottime = time[pktcount-1] - time[0];
tottime = tottime/60;
j = fmod(tottime,5.0);
tottime = tottime + (5.0 - j);

j=0.0;
temp = 0;
for (i=0; i<4; i++)
	portsort[i]=0;
i=0;
y=5;
a=0;
b=0;
c=0;
d=0;
at=0;
i1=0,i2=0,i3=0;

int m=0, m1 = 1;

temp = 0;
temp1 =0;
j = 5.0;

int arr[5000], connection[5000], same_connection, hash_number, found = 0;

for (i = 0; i < 5000; i++)
{
	arr[i] =0;
        connection[i] =0;
}
i =0 ;
for( j = 5.0; j<= tottime; j=j+5.0)
	{

		for( i = temp; i< pktcount; i++)
		{
			if((time[i]/60.0) < j )
			{


				temp1 ++;
				s = src[i];
				des = dst[i];
				sp = srcport[i];
				dp = dstport[i];

				hash_number = (((s^des)^sp)^dp);

				for (i2 = 0; i2 < 5000; i2++)
				{
					if (arr[i2] == hash_number)
					{
						found = 1;
						same_connection = i2;
						break;
					}


				}
				if (found == 1)
				{
					number = connection[same_connection];
					found =0;

				}
				else
				{
						portsort[0] = port1[a];
						portsort[1] = port2[a];
						portsort[2] = port3[a];
						portsort[3] = port4[a];

						for(l=0;l<4;l++)
						{
							int index_of_min = l;
							for(k=l;k<4;k++)
							{
								if(portsort[index_of_min] > portsort[k])
								{
									index_of_min = k;
								}
							}
							tem = portsort[l];
							portsort[l] = portsort[index_of_min];
							portsort[index_of_min] = tem;
						}

						if (portsort[0] == port1[a])
							number = 0;
						else if (portsort[0] == port2[a])
							number = 1;
						else if (portsort[0] == port3[a])
							number = 2;
						else if (portsort[0] == port4[a])
							number = 3;

						if (i1%5000 == 0)
							i1=0;

						arr[i1] = hash_number;
						connection[i1] = number;
						i1++;


				}

				switch(number)
				{
					case 0: {

							port1[a] = port1[a] + load[i]*8 + (40*8);
							break;
						}
					case 1: {

							port2[a] = port2[a] + load[i]*8 + (40*8);
							break;
						}
					case 2:
						{

							port3[a] = port3[a] + load[i]*8 + (40*8);
							break;
						}
					case 3:
						{

							port4[a] = port4[a] + load[i]*8 + (40*8);
							break;
						}
				}

			}
		}


		temp+= temp1;
		totaltime = time[temp-1] - time[temp-temp1];
        	port1[a] = port1[a] / (totaltime*1024);
		port2[a] = port2[a] / (totaltime*1024);
		port3[a] = port3[a] / (totaltime*1024);
		port4[a] = port4[a] / (totaltime*1024);
		maxdiff =0;
		temp1 = 0;
		portsort[0] = port1[a];
		portsort[1] = port2[a];
		portsort[2] = port3[a];
		portsort[3] = port4[a];

		for(l=0;l<4;l++)
		{
			int index_of_min = l;
			for(k=l;k<4;k++)
			{
				if(portsort[index_of_min] < portsort[k])
				{
					index_of_min = k;
				}
			}
			tem = portsort[l];
			portsort[l] = portsort[index_of_min];
			portsort[index_of_min] = tem;
		}
		maxdiff = portsort[0] - portsort[3];
		printf("%d-%d\t\t%f\t\t%f\t\t%f\t\t%f\t\t%f\n",y-5,y , port1[a],port2[a], port3[a], port4[a], maxdiff);
		for(l=0;l<4;l++)
			portsort[l]=0;
		a++;
		y+=5;
	}



    return 0;
}

