//============================================================================
// Name        : uudecode.cpp
// Author      : Michael C Gomez / Computer Engineering Student / MNSU.edu
// Instructor  : Dr. Dean Kelley
// Course      : CS 350 Network Architectures
// Assignment  :  Assignment One, Due 23:59 November 1, 2019.
// Compile Note: Use -std=c++11 to enforce c++ 2011's new string functions.
//============================================================================
#include <iostream>
using namespace std;
/*
 * MyFun.cpp // short for My functions.
 *
 *  Created on: Oct 29, 2019
 *	Author: michael c gomez
 *      	Computer Engineering Student
 *      DEVELOPED ON ECLIPSE IDE FOR UBUNTU 19.10.
 *      I am experienced in JAVA so C++ was interesting...to learn.
 *      I also used C++ How To Program (6th Ed.) by dietel to learn faster (since I already know object oriented etc...)
 *      I also used cplusplus.com for api reference on String and stream classes as well as your project instructions.
 *      I decided upon a MyFun.cpp and MyFun.h because I found it difficult to compile more than one int main() function
 *       per project in eclipse cdt like I was used to with java.
 *       Once I was done testing. I copied MyFun.cpp into two main files named uuencode.cpp and uudecode.cpp so that I dont
 *       have to worry about compiling multiple files and duplicate #includes.... geany in the TE315 lab was not friendly unless
 *       I had only one file to compile.
 *      Otherwise I prefer to leave it as MyFun.cpp and MyFun.h but I don't know how to compile those from command line with out
 *      ECLIPSE IDE in ubuntu.
 *      I FOUND my program worked well because of method decomposition... within an hour or two of learning streams after a few days
 *      making helper methods.
 *      However, as comments below note... I found my code was buggy on windows machine... both in geany compiling and
 *      on my personal Desktop Windows 10 machine with community version of windows visual studio in a C++ console application project...
 *
 *      .. no matter the windows environment I encountered infinte loop bug on decoder executable in windows when i copied my linux
 *      code over to windows. Line 217 or so describes my discovery and subsequent solution. Now it should
 *      compile fine on both windows and linux.
 */
#include<iostream>
#include<string>
#include<climits>
#include<fstream>

using namespace std;

class MyFun
{
	public:
	static int  uuencode()
	{

			string inputFileName;
			string outputFileName;
			string encodedFileName;
			cout<<"File to encode: ";
			cin>>inputFileName;
			cout<<"Final destination file name: ";
			cin>>outputFileName;
			cout<<"Encoded output file name: ";
			cin>>encodedFileName;
			cout<<"Bytes per line: 45\n"<<endl;

					ifstream infile;
					infile.open(inputFileName,ios::in|ios::binary);
					if(!infile.is_open())
					{
						cerr<<"Failed to open!"<<endl;
						infile.close();
						return -1;
					}

					infile.seekg(0,ios::end);
					int filesize=infile.tellg(); 
					infile.seekg(0);


					ofstream outfile(encodedFileName,ios::binary);

			cout<<"\tEncoded: "<<inputFileName<<endl;
			cout<<"\tTo: "<<encodedFileName<<endl;
			cout<<"\tByte count: "<<filesize<<"\n\n"<<endl;

			//========================END ADMINISTRATIVE PAPER WORK=================
			//============================BEGIN ENCODING PROCESS=================

			char decodedLine[45];
			char encodedLine[63];//60+sizechar+newlinechar+ flexibility for debuging carriage return.


			outfile<<"begin 644 "<<outputFileName;
			outfile.put(10);//newline

			if( DEBUG)
			{
						cout<< "gcount "<<infile.gcount()<<endl;
						cout<< "EOF()?"<<infile.eof()<<endl;
			}

			while(!infile.eof())
			{
				initNZeros(decodedLine,0,45);
				initNZeros(encodedLine,0,63);

				infile.read(decodedLine,45);
				int lineCount=infile.gcount();
				 encodeLine(decodedLine,encodedLine,lineCount);
				int chunks= getChunkCount(lineCount);
				for(int i=0;i<((chunks*4)+1+1);i++)
				outfile.put(encodedLine[i]);
				//cout<<endl;

			}
			//initNZeros(decodedLine,0,45);
			//initNZeros(encodedLine,0,62);
			// encodeLine(decodedLine,encodedLine,0);
			//for(int i=0;i<62;i++)
			//cout<<encodedLine[i];
			//cout<<endl;
			//cout<<"BREAQK"<<endl;
			outfile.put(96);//grahve character
			outfile.put(10);//newline character.
			outfile<<"end";


	//		 encodeLine(decodedLine,encodedLine,infile.gcount());
	//
	//		for(int i=0;i<62;i++)
	//		cout<<encodedLine[i];
	//		cout<<endl;
	//
	//
	//				infile.read(decodedLine,45);
	//				cout<<"begin 644 "<<encodedFileName<<endl;
	//
	//				cout<< "gcount "<<infile.gcount()<<endl;
	//				cout<< "EOF()?"<<infile.eof()<<endl;
	//
	//				 encodeLine(decodedLine,encodedLine,infile.gcount());
	//
	//				for(int i=0;i<62;i++)
	//				cout<<encodedLine[i];
	//				cout<<endl;


			infile.close();
			outfile.close();
			return 0;
	}
	static void  initNZeros(char array[],int index,int n)
	{
		for(int i=0;i<n;i++)
		{
			array[index+i]=0;
		}
	}
	static int  getChunkCount(int linebytes)
	{
		int remainder=linebytes%3;//possible values 0,1,2
		int totalChunks=linebytes/3;
		totalChunks+=((remainder)?1:0);
		return totalChunks;
	}
	static void  encodeLine(char decodedLine[],char encodedLine[],int lineBytes)//expect 45 and 63 array elements respectively.
	{
				//int remainder=lineBytes%3;//possible values =0,1,2
				int totalChunks=getChunkCount(lineBytes);
				int totalFours=totalChunks;


				int chunks=0;
				encodedLine[0]=lineBytes+32;//set the linebyte count
				if(encodedLine[0]==32)//handle space encoded linebyte with replacement of grahve.
				{
					encodedLine[0]=96;
				}

				encodedLine[(totalFours*4)+1]=10;//set newline character to end of the line.

				for(;chunks<totalChunks;chunks++)
				{
					 encode3BytesTo4Bytes(decodedLine,chunks*3,encodedLine,(chunks*4)+1);
				}

				if( DEBUG)
				{
								cout<<"TOTAL CHUNKS CALCULATED"<<totalChunks<<endl;
								cout<<"decodedLine = "<< toNCharDecimal(decodedLine,0,45)<<endl;
								cout<<"encodedLine = "<< toNCharDecimal(encodedLine,0,63)<<endl;
				}

	}

	static int  uudecode()
	{
		string target;
		cout<<"File to decode: ";
		cin>>target;//bird6-dk.uue


		ifstream infile;
						infile.open(target,ios::in|ios::binary);
						if(!infile.is_open())
						{
							cerr<<"Failed to open!"<<endl;
							infile.close();
							return -1;
						}

						infile.seekg(0,ios::end);
						int filesize=infile.tellg();
						infile.seekg(0);
							if(DEBUG)
								cout<<"FILE SIZE = "<<filesize<< " bytes."<<endl;

						char encodedLine[63];
						 initNZeros(encodedLine,0,63);
						char decodedLine[45];

						infile.getline(encodedLine,63,'\n');
						//terminates before index 62 if \n found
						//on windows determined if by accident another
						//implementation uses carriage return and line feed chars 13 and 10
						//as is used on windows instead of the 10 just used on linux, this will pick it up.
						//early trials i had compiling my code on windows resulted in infinite loop
						//in the decoder.
						//it was found that i did not set the output stream mode to binary mode so
						//os dependent new line was attached to my encoder output...despite my best efforts
						//to restrict it to just value 10.
						// value 13,10 would cause the getline function to not read any characters because it
						//did not encounter a \n character until index 62. But my encoded line array was 62 elements
						//long and only went to index 61.. so it would infinitely read getline and read nothing.
						// SETTING outfile to binary mode solved this bug my encoder caused on windows when i originally
						//compiled this on windows. Setting encoded Line to 63 makes sure infinite loop does not occur if 13,10 occurs.
						// my encoded is now fixed to output just 10 and so only other people's encodings might accidentally do 13,10
						// my decoder will not infinite loop on their files now.
						// NOTE MY CODE WAS DEVELOPED ON UBUNTU of my dual boot laptop. it worked with little bugs until I tried
						//compilling it on windows... I finally solved this "windows" bug. will turn in now.. 3 hours before deadline.

						string firstline(encodedLine);

						int firstSpace=firstline.find(' ');
						int secondSpace=firstline.rfind(' ');
							if(DEBUG)
							{
								cout<<"gcount"<<infile.gcount();
								cout<<"STRING SIZE"<<firstline.size()<<endl;
								cout<<"FIRST SPACE = "<<firstSpace<<" SECOND SPACE = "<<secondSpace<<endl;
								cout<<"STRING::NPOS="<<(int)(string::npos)<<endl;
							}

						if(firstSpace>62||firstSpace<0||secondSpace>62||secondSpace<0)
						{
							cerr<<"Not a valid encoding of the Unix-to-Unix format.";
							return -1;
						}
						string begin=firstline.substr(0,firstSpace);
						if(DEBUG)
							cout<<"First Token = "<<begin<<endl;
						string permissions=firstline.substr(firstSpace+1,secondSpace-firstSpace);
						if(DEBUG)
							cout<<"Second Token = "<<permissions<<endl;
						string destination=firstline.substr(secondSpace+1,firstline.size()-secondSpace);
						if(DEBUG)
							cout<<"Third  Token = "<<destination<<endl;

						if(DEBUG)
							cout<<"1stLine ARRAY = "<< toNCharDecimal(encodedLine,0,63)<<endl;

						ofstream outfile(destination,ios::binary);
						cout<<"\tOpened for output: "<< destination<<endl;
						int loopcount=0;
						//int chunks=0;
						int expectedBytesForThisLine=0;
						for(;!infile.eof();loopcount++)
						{
							 initNZeros(encodedLine,0,63);
							 initNZeros(decodedLine,0,45);
							infile.getline(encodedLine,63,'\n');

							if(DEBUG)
							{
								cout<<"gcount"<<infile.gcount();
								cout<<" "<<loopcount<<"Encoded Array= "<< toNCharDecimal(encodedLine,0,62)<<endl;
							}

							if(encodedLine[0]==96)
							{
								break;
							}
							else
							{
								expectedBytesForThisLine=encodedLine[0]-32;
							}
							 decodeLine(encodedLine,decodedLine,expectedBytesForThisLine);
							if(DEBUG)
								cout<<" "<<loopcount<<"Decoded Array= "<< toNCharDecimal(decodedLine,0,45)<<endl;
							for(int i=0;i<expectedBytesForThisLine;i++)
								outfile.put(decodedLine[i]);
						}
						if(DEBUG)
							cout<<"TELLG()"<<outfile.tellp();
						cout<<"\tWrote: "<<outfile.tellp()<<" bytes";
						outfile.close();


						// initNZeros(encodedLine,0,62);
						//infile.getline(encodedLine,62,'\n');
						//cout<<"gcount"<<infile.gcount();
						//cout<<"2ndLine ARRAY = "<< toNCharDecimal(encodedLine,0,62)<<endl;



		infile.close();
		return 0;
	}
	static void decodeLine(char encodedLine[],char decodedLine[],int lineBytes)//expected 62 bytes and 45 bytes respectively
	{
		int chunkTotal= getChunkCount(lineBytes);
		for(int i=0;i<chunkTotal;i++)
		{
			 decode4BytesTo3Bytes(encodedLine,(i*4)+1,decodedLine,i*3);
		}

	}
	static void  encode3BytesTo4Bytes(char decoded[],int dIndex,char encoded[],int eIndex)
	{
		int temp= extract24BitIntFrom3CharArray(decoded,dIndex);
		 fill4CharArrayFrom24BitInt(temp,encoded,eIndex);
		 filterInASCII(encoded,eIndex);
	}
	static void  decode4BytesTo3Bytes(char encoded[],int eIndex,char decoded[],int dIndex)
	{
		 filterOutASCII(encoded,eIndex);
		int temp= extract24BitIntFrom4CharArray(encoded,eIndex);
		 fill3CharArrayFrom24BitInt(temp,decoded,dIndex);
	}
	static unsigned char  extractByte0From24Bits(unsigned int input)//left most
	{
		input=input>>16;
		unsigned int mask=0x000000ff;
		return input&mask;
	}
	static unsigned char  extractByte1From24Bits(unsigned int input)
	{
		input=input>>8;
		unsigned int mask=0x000000ff;
		return input&mask;
	}
	static unsigned char  extractByte2From24Bits(unsigned int input)
	{
		input=input>>0;
		unsigned int mask=0x000000ff;
		return input&mask;
	}
	/*unsigned char  extractByte3(unsigned int input)//right most
	{
		unsigned mask=0x000000ff;
		return input&mask;
	}
	*/
	static unsigned int  extract24BitIntFrom3CharArray(char input[],int index)//minimum three byte array; //if length>4 it is ignored
	{
		unsigned int byte0=input[index+0];
		unsigned int byte1=input[index+1];
		unsigned int byte2=input[index+2];
		//unsigned int byte3=input[index+3];

		byte0=((byte0<<16)&0x00ff0000);
		byte1=((byte1<<8) &0x0000ff00);
		byte2=((byte2<<0) &0x000000ff);


		return byte0|byte1|byte2;
	}
	static char*  fill3CharArrayFrom24BitInt(unsigned int input, char dest[],int index)// return array three long min
	{
		dest[index+0]=extractByte0From24Bits(input);
		dest[index+1]=extractByte1From24Bits(input);
		dest[index+2]=extractByte2From24Bits(input);
		//dest[index+3]=extractByte3(input);
		return dest;
	}


	static unsigned char  extractBits0to5From24Bits(unsigned int input)//LEFT MOST
	{
		int masq=0x0000003F; //00000000 00000000 00000000 00111111
		int output=(input>>18)&masq;
		return output;
	}
	static void  testExtractBits0to5From24Bits()
	{
		int number=0;
		cout<<"Enter an integer"<<endl;
		cin>>number;
		unsigned char yup= extractBits0to5From24Bits(number);
		cout<<"The binary for integer is  "<< to24BitBinary8BitChunks(number)<<endl;
		cout<<"The binary for Bits 0-5 is "<< to24BitBinary8BitChunks(yup)<<endl;

	}
	static unsigned char  extractBits6to11From24Bits(unsigned int input)
	{
		int masq=0x0000003F; //00000000 00000000 00000000 00111111
		int output=(input>>12)&masq;
		return output;
	}
	static void  testExtractBits6to11From24Bits()
	{
		int number=0;
		cout<<"Enter an integer"<<endl;
		cin>>number;
		unsigned char yup= extractBits6to11From24Bits(number);
		cout<<"The binary for integer is  "<< to24BitBinary8BitChunks(number)<<endl;
		cout<<"The binary for Bits 6-11 is "<< to24BitBinary8BitChunks(yup)<<endl;

	}
	static unsigned char  extractBits12to17From24Bits(unsigned int input)
	{
			int masq=0x0000003F; //00000000 00000000 00000000 00111111
			int output=(input>>6)&masq;
			return output;
	}
	static void  testExtractBits12to17From24Bits()
	{
		int number=0;
		cout<<"Enter an integer"<<endl;
		cin>>number;
		unsigned char yup= extractBits12to17From24Bits(number);
		cout<<"The binary for integer is    "<< to24BitBinary8BitChunks(number)<<endl;
		cout<<"The binary for Bits 12-17 is "<< to24BitBinary8BitChunks(yup)<<endl;

	}
	static unsigned char  extractBits18to23From24Bits(unsigned int input)//RIGHT MOST
	{
				int masq=0x0000003F; //00000000 00000000 00000000 00111111
				int output=(input>>0)&masq;
				return output;
	}
	static void  testExtractBits18to23From24Bits()
	{
		int number=0;
		cout<<"Enter an integer"<<endl;
		cin>>number;
		unsigned char yup= extractBits18to23From24Bits(number);
		cout<<"The binary for integer is    "<< to24BitBinary8BitChunks(number)<<endl;
		cout<<"The binary for Bits 18-23 is "<< to24BitBinary8BitChunks(yup)<<endl;

	}
	static char*  fill4CharArrayFrom24BitInt(unsigned int input, char array[] ,int index)
	{
		input=input&0x00ffffff;
		unsigned char byte0= extractBits0to5From24Bits(input);
		unsigned char byte1= extractBits6to11From24Bits(input);
		unsigned char byte2= extractBits12to17From24Bits(input);
		unsigned char byte3= extractBits18to23From24Bits(input);
		array[index+0]=byte0;
		array[index+1]=byte1;
		array[index+2]=byte2;
		array[index+3]=byte3;
		return array;
	}
	static unsigned int  extract24BitIntFrom4CharArray(char input[],int index)
	{
		int workingBits=0;

		unsigned int byte0=((input[index+0]&0x3F)<<18);
		unsigned int byte1=((input[index+1]&0x3F)<<12);
		unsigned int byte2=((input[index+2]&0x3F)<<6);
		unsigned int byte3=((input[index+3]&0x3F)<<0);

		workingBits=byte0|byte1|byte2|byte3;

		return workingBits;
	}
	static void  test4CharTo24BitBackandForth()
	{
		int number=0;
		cout<<"enter an integer: "<<endl;
		cin>>number;
		string binaryNumber32= to32BitBinary8BitChunks(number);
		string binaryNumber24= to24BitBinary8BitChunks(number);
		cout<<"In 32 Bit binary this number is: "<<binaryNumber32<<endl;
		cout<<"In 24 Bit binary this number is: "<<binaryNumber24<<endl;
		cout<<"fill4CharArrayFrom24BitInt("<<number<<");"<<endl;
		char codedArray[4];
		 fill4CharArrayFrom24BitInt(number,codedArray,0);
		string binaryCodedArray= toFourCharBinary(codedArray,0);
		string decimalCodedArray= toFourCharDecimal(codedArray,0);
		cout<<"The resulting array has the following representations:"<<endl;
		cout<<"In Binary:  "<<binaryCodedArray<<endl;
		cout<<"In Decimal: "<<decimalCodedArray<<endl;
		cout<<"This concludes this test function: test4CharTo24BitBackandForth"<<endl;

	}

	static char*  filterInASCII(char input[],int index)
	{
		int i=0;
		for(;i<4;i++)
		{
			input[index+i]+=(unsigned char)32;
			if(input[index+i]==(unsigned char)32)
				input[index+i]=(unsigned char)96;
		}
		return input;
	}
	static char*  filterOutASCII(char input[],int index)
	{
		int i=0;
		for(;i<4;i++)
		{
			if(input[index+i]==(unsigned char)96)
			{
				input[index+i]=(unsigned char)32;
			}
			input[index+i]-=(unsigned char)32;

		}
		return input;
	}
	static std::string  to32BitBinary8BitChunks(unsigned int input)
	{
		char binary[33];
		// we want index 31 to be LSB
		// we want index 0 to be MSB
		// we want index 32 to be NULL char.
		int i=31;
		binary[32]='\0';
		for (;i>=0;i--)
			binary[i]='0';//ascii code for symbol zero is 48

		unsigned int mask=0x00000001;
		i=31;
		for (;i>=0;i--)
			binary[i]=binary[i]+ ((input>>(31-i))&mask);
		string output(binary);
		output.insert(24," ");
		output.insert(16," ");
		output.insert(8," ");
		return output;
	}
	static std::string  to24BitBinary8BitChunks(unsigned int input)
	{
		char binary[25];
		// we want index 31 to be LSB
		// we want index 0 to be MSB
		// we want index 32 to be NULL char.
		int i=23;
		binary[24]='\0';
		for (;i>=0;i--)
			binary[i]='0';//ascii code for symbol zero is 48

		unsigned int mask=0x00000001;
		i=23;
		for (;i>=0;i--)
			binary[i]=binary[i]+ ((input>>(23-i))&mask);
		string output(binary);
		//output.insert(24," ");
		output.insert(16," ");
		output.insert(8," ");
		return output;
	}
	static std::string  toFourCharBinary(char source[],int index)
	{
		string output;


			int i=0;
			int c=0;
			char array[9];
			for(;c<8;c++)
			array[c]='0';
			array[8]='\0';
			c=7;
			for(;i<4;i++)
			{
				for(;c>=0;c--)
				{
					array[c]=array[c]+((source[i]>>(7-c))&0x01);
				}
				output.append(" ");
				output.append(array);
				c=0;
				for(;c<8;c++)
				{
					array[c]='0';
				}
				array[8]='\0';
			}


			output.append("}");
			output.insert(0,"{");
			return output;
	}
	static std::string  toFourCharDecimal(char source[],int index)
	{
			string output("{");
			output.append(std::to_string((unsigned char)source[index+0]));
			output.append(",");
			output.append(std::to_string((unsigned char)source[index+1]));
			output.append(",");
			output.append(std::to_string((unsigned char)source[index+2]));
			output.append(",");
			output.append(std::to_string((unsigned char)source[index+3]));
			output.append("}");
			return toNCharDecimal(source,index,4);
	}
	static std::string  toNCharDecimal(char input[],int index,int n)
	{
		string output;
		int i=0;
		output.append("{ ");
		for(;i<n;i++)
		{
			output.append(std::to_string((unsigned char)input[index+i]));
			if(i==(n-1))
				output.append("}");
			else
				output.append(" ,");
		}
		return output;
	}
	static std::string  toThreeCharBinary(char source[],int index)
	{
			string output= to24BitBinary8BitChunks( extract24BitIntFrom3CharArray(source,index));
			output.append("}");
			output.insert(0,"{");
			return output;
	}
	static std::string  toThreeCharDecimal(char source[],int index)
	{
		string output("{");
		output.append(std::to_string((unsigned char)source[index+0]));
		output.append(",");
		output.append(std::to_string((unsigned char)source[index+1]));
		output.append(",");
		output.append(std::to_string((unsigned char)source[index+2]));
		output.append("}");
		return output;
	}
	static void  testStringRep()
	{
		cout<<"Enter an integer:= "<<endl;
		int number=0;
		cin>>number;
		cout<<"You Entered "<<number<<". Which has a binary value of:"<<endl;
		cout<< to24BitBinary8BitChunks(number)<<endl;
		char array[3];
		array[0]=0;
		array[1]=0;
		array[2]=0;
		 fill3CharArrayFrom24BitInt(number,array,0);
		cout<<"In array form, the binary values are as follows"<<endl;
		cout<< toThreeCharBinary(array,0)<<endl;
		cout<<"In Decimal form, the values are as follows"<<endl;
		cout<< toThreeCharDecimal(array,0)<<endl;

	}
	static void  printSystemLimits()
	{
		cout<< "CHAR_BIT   = "<< CHAR_BIT<<endl;
		cout<< "SCHAR_MIN  = "<< SCHAR_MIN<<endl;
		cout<< "SCHAR_MAX  = "<< SCHAR_MAX<<endl;
		cout<< "UCHAR_MAX  = "<< UCHAR_MAX<<endl;
		cout<< "CHAR_MIN   = "<< CHAR_MIN<<endl;
		cout<< "CHAR_MAX   = "<< CHAR_MAX<<endl;
		cout<< "MB_LEN_MAX = "<< MB_LEN_MAX<<endl;
		cout<< "SHRT_MIN   = "<< SHRT_MIN<<endl;
		cout<< "SHRT_MAX   = "<< SHRT_MAX<<endl;
		cout<< "USHRT_MAX  = "<< USHRT_MAX<<endl;
		cout<< "INT_MIN    = "<< INT_MIN<<endl;
		cout<< "INT_MAX    = "<< INT_MAX<<endl;
		cout<< "UINT_MAX   = "<< UINT_MAX<<endl;
		cout<< "LONG_MIN   = "<< LONG_MIN<<endl;
		cout<< "LONG_MAX   = "<< LONG_MAX<<endl;
		cout<< "ULONG_MAX  = "<< ULONG_MAX<<endl;
		cout<< "LLONG_MIN  = "<< LLONG_MIN<<endl;
		cout<< "LLONG_MAX  = "<< LLONG_MAX<<endl;
		cout<< "ULLONG_MAX = "<< ULLONG_MAX<<endl;
	}
	private:
	static const int DEBUG=0;

};

int main()
{
	return MyFun::uudecode();
}
