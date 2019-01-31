#ifndef CHIP8
#define CHIP8

class Chip8{
	public:
		unsigned short opcode;
		unsigned char memory[4096];
		unsigned char V[16];
		unsigned short indexreg;
		unsigned short pc;
		unsigned char pixels[64 * 32];
		unsigned char delayTimer;
		unsigned char soundTimer;
		unsigned short stack [16];
		unsigned short sp;
		unsigned char key[16];

		void initialize();
		void emulateCycle();
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
};
		void runOp(unsigned short);
};

void Chip8::initialize(){
 	pc = 0x200;
	opcode = 0;
	indexreg = 0;
	sp = 0;

	for(int i = 0; i < 80; i++)
		memory[i] = chip8_fontset[i];

}

void Chip8::emulateCycle(){
	this->opcode = memory[pc] << 8 | memory[pc + 1];

	switch (opcode & 0xF000){
		case 0x000:
			switch(opcode & 0x00F){
				case 0x000:
					//0x00E0 clears the screen
				break;

				case 0x00E:
					//0x00EE Returns from subroutine
				break;
			}
		break;

		case 0x2000:
			stack[sp] = pc;
			++sp;
			pc = opcode & 0xFFF;
		break;
		case 0x8000:
		 switch(opcode & 0x000F){
		 
			case 0x0004:
				//TODO
			break;
		 }

		 break;

		case 0xA000:
			indexreg = opcode & 0x0FFF;
			pc += 2;
		break;


		default:
			printf("Unknown opcode: 0x%X\n", opcode);
	}

	if(delayTimer > 0)
		--delayTimer;

	if(soundTimer > 0){
	
		if(soundTimer == 1)
			printf("BEEP!\n");

		--soundTimer;
	}
}







#endif
