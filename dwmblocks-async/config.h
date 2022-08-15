#define CMDLENGTH 45
#define DELIMITER "  "
#define CLICKABLE_BLOCKS

const Block blocks[] = {
	BLOCK("sbnettraf", 1, 16),
	BLOCK("sbnet", 5, 4),
	BLOCK("sbvol", 0, 10),
	BLOCK("date +'['%a,' '%d' '%b' '%H:%M:%S']'", 1, 0)
};
