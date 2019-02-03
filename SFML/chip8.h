#ifndef CHIP8
#define CHIP8

#include <stdlib.h>
#include <iostream>

class Chip8{
	public:
		bool drawFlag;
		unsigned char memory[4096];
		unsigned char V[16];
		unsigned char pixels[2048];
		unsigned char key[16];
		unsigned short stack [16];
		unsigned short pc;
		unsigned short indexreg;
		unsigned short sp;
		unsigned short opcode;
		unsigned char soundTimer;
		unsigned char delayTimer;


		void initialize();
		void emulateCycle();
		bool loadApplication(const char *);
		void debugRender();
	private:
		unsigned char chip8_fontset[80] =
{
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};};

void Chip8::initialize(){
 	pc = 0x200;
	opcode = 0;
	indexreg = 0;
	sp = 0;

	for(int i = 0; i < 2048; i++)
		pixels[i] = 0;

	for(int i = 0; i < 16; ++i)
		stack[i] = 0;

	for(int i = 0; i < 16; ++i)
		key[i] = V[i] = 0;

	for(int i = 0; i < 4096; ++i)
		memory[i] = 0;

	for(int i = 0; i < 80; i++)
		memory[i] = chip8_fontset[i];

	drawFlag = true;
}

void Chip8::emulateCycle(){
	opcode = memory[pc] << 8 | memory[pc + 1];

	switch (opcode & 0xF000){
		case 0x000:
			switch(opcode & 0x000F){
				case 0x0000:
					for(int i = 0; i < 2048; ++i)
						pixels[i] = 0x0;
					drawFlag = true;
					pc += 2;
				break;

				case 0x000E:
					--sp;
					pc = stack[sp];
					pc += 2;
				break;

				default:
					printf( "Unknown opcode 0x0000: 0x%X\n", opcode);
				break;
			}
		break;

		case 0x1000:

			pc = opcode & 0x0FFF;
		break;

		case 0x2000:
			stack[sp] = pc;
			++sp;
			pc = opcode & 0x0FFF;
		break;

		case 0x3000:
			if(V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
				pc += 4;
			 else
				pc += 2;

		break;

		case 0x4000:
			if(V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
				pc += 4;
			 else
				pc += 2;

		break;

		case 0x5000:
			if(V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4])
				pc += 4;
			else
				pc += 2;

		break;

		case 0x6000:
			V[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF);
			pc += 2;
		break;

		case 0x7000:
			V[(opcode & 0x0F00) >> 8] += (opcode & 0x00FF);
			pc += 2;
		break;

		case 0x8000:
			switch(opcode & 0x000F){
				case 0x0000:
					V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
					pc += 2;
				break;

				case 0x0001:
					V[(opcode & 0x0F00) >> 8] |=	V[(opcode & 0x00F0) >> 4];
					pc += 2;
				break;

				case 0x0002:
					V[(opcode & 0x0F00) >> 8] &= V[(opcode & 0x00F0) >> 4];
					pc += 2;
				break;

				case 0x0003:
					V[(opcode & 0x0F00) >> 8] ^=  V[(opcode & 0x00F0) >> 4];
					pc += 2;
				break;

				case 0x0004:
					if(V[(opcode & 0x00F0) >> 4] > (0xFF - V[(opcode & 0x0F00) >> 8]))
						V[0xF] = 1; //carry
					else
						V[0xF] = 0;
						V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
						pc += 2;
				break;

				case 0x0005: // 8XY5 - VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
					if(V[(opcode & 0x00F0) >> 4] > V[(opcode & 0x0F00) >> 8])
						V[0xF] = 0;
					else
						V[0xF] = 1;
						V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
						pc += 2;
				break;

				case 0x0006:
					V[0xF] = V[(opcode & 0x0F00) >> 8] & 0x1;
					V[(opcode & 0x0F00) >> 8] >>= 1;
					pc += 2;
				break;

				case 0x0007: // 8XY7 Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
					if(V[(opcode & 0x00F0) >> 4] < V[(opcode & 0x0F00) >> 8])
						V[0xF] = 0;
					else
						V[0xF] = 1;
						V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
						pc += 2;
				break;

				case 0x000E:
					V[0xF] = V[(opcode & 0x0F00) >> 8] >> 7;
					V[(opcode & 0x0F00) >> 8] <<= 1;
					pc += 2;
				break;
		 }
		break;

		case 0x9000:
			if (V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0)] >> 4)
				pc += 4;
			else
				pc += 2;
		break;

		case 0xA000:
			indexreg = opcode & 0x0FFF;
			pc += 2;
		break;

		case 0xB000:
			pc = V[0] + (opcode & 0x0FFF);
		break;

		case 0xC000: // Generates random number, luls
			V[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF) & (rand() & 0xFF);
			pc+=2;
		break;

		case 0xD000:{
			unsigned short x = V[(opcode & 0x0F00) >> 8];
			unsigned short y = V[(opcode & 0x00F0) >> 4];
			unsigned short height = (opcode & 0x000F);
			unsigned short pixel;

			V[0xF] = 0;

			for(int yline = 0; yline < height; yline++){

				pixel = memory[indexreg + yline];
				for(int xline = 0; xline < 8; xline++){

					if((pixel & (0x80 >> xline)) != 0){

		        if(pixels[(x + xline + ((y + yline) * 64))] == 1)
		          V[0xF] = 1;
		        pixels[x + xline + ((y + yline) * 64)] ^= 1;
	      	}
				}
			}
			drawFlag = true;
			pc += 2;
		}
		break;

		case 0xE000:
			switch (opcode & 0x00FF){

				case 0x009E:
					if (key[V[(opcode & 0x0F00) >> 8]] != 0)
						pc += 4;
					else
						pc += 2;
				break;

				case 0x00A1:
					if (key[V[(opcode & 0x0F00) >> 8]] == 0)
						pc += 4;
					else
						pc += 2;
				break;

				default:
				printf("Opcode E - %X not recognized\n",(opcode & 0x00F) );

				break;
			}
		break;

		case 0xF000:
			switch (opcode & 0x00FF){

				case 0x0007:
					V[(opcode & 0x0F00) >> 8] = delayTimer;
					pc+=2;

				break;

				case 0x000A:{
					bool keyPress = false;

					for(int i = 0; i < 16; ++i){
						if(key[i] != 0){
							V[(opcode & 0x0F00) >> 8] = i;
							keyPress = true;
						}
					}

					if(!keyPress)
						return;

					pc += 2;
				}
				break;

				case 0x0015:
					delayTimer = V[(opcode & 0x0F00) >> 8];
					pc+=2;
				break;

				case 0x0018:
					soundTimer = V[(opcode & 0x0F00) >> 8];
					pc+=2;
				break;

				case 0x001E:
				if(indexreg + V[(opcode & 0x0F00) >> 8] > 0xFFF)
					V[0xF] = 1;
				else
					V[0xF] = 0;
				indexreg += V[(opcode & 0x0F00) >> 8];
				pc += 2;
				break;

				case 0x0029: // FX29: Sets I to the location of the sprite for the character in VX. Characters 0-F (in hexadecimal) are represented by a 4x5 font
					indexreg = V[(opcode & 0x0F00) >> 8] * 0x5;
					pc += 2;
				break;

				case 0x0033:
					memory[indexreg] = V[(opcode & 0x0F00) >> 8] / 100;
  				memory[indexreg + 1] = (V[(opcode & 0x0F00) >> 8] / 10) % 10;
  				memory[indexreg + 2] = (V[(opcode & 0x0F00) >> 8] % 100) % 10;
  				pc += 2;
				break;

				case 0x0055: // FX55: Stores V0 to VX in memory starting at address I
					for (int i = 0; i <= ((opcode & 0x0F00) >> 8); ++i)
						memory[indexreg + i] = V[i];

					// On the original interpreter, when the operation is done, I = I + X + 1.
					indexreg += ((opcode & 0x0F00) >> 8) + 1;
					pc += 2;
				break;

				case 0x0065: // FX65: Fills V0 to VX with values from memory starting at address I
					for (int i = 0; i <= ((opcode & 0x0F00) >> 8); ++i)
						V[i] = memory[indexreg + i];

					// On the original interpreter, when the operation is done, I = I + X + 1.
					indexreg += ((opcode & 0x0F00) >> 8) + 1;
					pc += 2;
				break;

			}
		break;


		default:
			printf("Unknown opcode: 0x%X\n", opcode);
		break;
	}

	if(delayTimer > 0)
		--delayTimer;

	if(soundTimer > 0){

		if(soundTimer == 1)
			printf("BEEP!\n");

		--soundTimer;
	}
}

void Chip8::debugRender()
{
	// Draw
	for(int y = 0; y < 32; ++y)
	{
		for(int x = 0; x < 64; ++x)
		{
			if(pixels[(y*64) + x] == 0)
				printf("O");
			else
				printf(" ");
		}
		printf("\n");
	}
	printf("\n");
}

bool Chip8::loadApplication(const char * filename){
	initialize();
	printf("Loading: %s\n", filename);

	// Open file
	FILE * pFile = fopen(filename, "rb");
	if (pFile == NULL)
	{
		fputs ("File error", stderr);
		return false;
	}

	// Check file size
	fseek(pFile , 0 , SEEK_END);
	long lSize = ftell(pFile);
	rewind(pFile);
	printf("Filesize: %d\n", (int)lSize);

	// Allocate memory to contain the whole file
	char * buffer = (char*)malloc(sizeof(char) * lSize);
	if (buffer == NULL)
	{
		fputs ("Memory error", stderr);
		return false;
	}

	// Copy the file into the buffer
	size_t result = fread (buffer, 1, lSize, pFile);
	if (result != lSize)
	{
		fputs("Reading error",stderr);
		return false;
	}

	// Copy buffer to Chip8 memory
	if((4096-512) > lSize)
	{
		for(int i = 0; i < lSize; ++i)
			memory[i + 512] = buffer[i];
	}
	else
		printf("Error: ROM too big for memory");

	// Close file, free buffer
	fclose(pFile);
	free(buffer);

	return true;
}


#endif
