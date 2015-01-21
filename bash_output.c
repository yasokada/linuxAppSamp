#include <stdio.h>

/*
[bastest.sh]
#!/bin/env bash
echo "This_is_test"
*/

int main(void)
{
	char buf[100];
	int size;

	FILE *pipe = popen("./bashtest.sh", "r");
	if (pipe == NULL) {
		printf("error\n");
		return;
	}

	while(!feof(pipe)) {
		size = (int)fread(buf, 1, 100, pipe);
	}

	pclose(pipe);

	printf("Output:%s", buf);
	// Output: This_is_test<LF>
}
