#include <stdio.h>
#include <ctype.h>
#include <string.h>

int pow_(int a,int b)
{
	int f = 1;
	while(b--)
	{
		 f *= a;
	}
	return f;
}

float to_decimal(char *hex)
{
	float num = 0;
	int p = 0;
  // partition
  char float_part[100];
  char hex_num[100];
  int i;
  for(i=0;hex[i]!='.' && i<strlen(hex);i++)
  {
    hex_num[i]=hex[i];
  }
  hex_num[i]='\0';
  if( i <strlen(hex))
  {
    //float part is there
    i++;
    int j = 1;
    for(i;i <strlen(hex);i++,j++)
    {
      if(hex[i]>=48 && hex[i]<=57)
      {
        num += ((float)(hex[i]-48)/(float)(pow_(16,j)));
      }
      else
      {
        num += ((float)(hex[i]-'a'+10)/(float)(pow_(16,j)));
      }
    }
  }
	// printf("\nconverting %s \n",hex_num);
	for(int i=strlen(hex_num)-1;i>=0;i--,p++)
	{
		if(isdigit(hex_num[i]))
		{
			num += (hex_num[i]-'0')*pow_(16,p);
		}
		else
		{
			num += (hex_num[i]-'a'+10)*pow_(16,p);
		}
	}
	// printf("%d converted\n",num);
	return num;
}

void to_hex(float num_h,char* num_hex)
{
	//printf("\nconverting to hex : %d\n",num);
  int num = (int)(num_h);
  num_h = num_h - num;
  printf("\n%f num_h %d num\n",num_h,num);
	char hex_num[100];
	int i=0;
	while(num)
	{
		int rem = num%16;
		num /= 16;
		if(rem<10)
		{
			hex_num[i]=rem+48;
		}
		else
		{
			hex_num[i] = rem-10 + 'a';
		}
		i++;
	}
	hex_num[i]='\0';
	// reverse
	char final_hex[100];
	for(int i=0;i<strlen(hex_num);i++)
	{
		final_hex[strlen(hex_num)-1-i] = hex_num[i];
	}
	final_hex[strlen(hex_num)]='\0';
   i = strlen(hex_num) + 1;
	//printf("\ndecimal to hex : %s\n",final_hex);
  if(num_hex)
  {
    strcat(final_hex,".");
    int count = 0;
    while(num_h != 16 && count<= 20)
    {
      num_h = num_h * 16;
      int first_part = num_h;
      printf("%d is first part\n",first_part);
      if(num_h > 0 && num_h != 16)
      {
          num_h = num_h - first_part;
          printf("after removing first part : %f and first part %d\n",num_h,first_part);
      }
      printf("%f is num_h \n",num_h);
      if(first_part <= 9)
      {
        final_hex[i++] = first_part+48;
      }
      else
      {
        final_hex[i++] = first_part - 10 + 'a';
      }
      count++;
    }
    final_hex[i]='\0';
  }
	strcpy(num_hex,final_hex);
}

void main()
{
  char num[100];
  float f;
  printf("\nEnter decimal number : ");
  scanf("%f",&f);
  to_hex(f,num);
  printf("\nHexadecimal equivalent is : %s\n",num);
}
