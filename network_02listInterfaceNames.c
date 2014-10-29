#include <stdio.h>
#include <stdlib.h>
#include <net/if.h>
#include <stdbool.h> // for bool

// Lists interface names using if_nameindex()
// 
// Thils will show [pan0] also on CentOS6.5,
// while [pan0] is not shown when using ioctl()
//

static bool isLast(struct if_nameindex *p)
{
	return ( p->if_name == NULL && 
		p->if_index == 0);
}

int main(void)
{
	struct if_nameindex *phead;
	struct if_nameindex *pitr;

	phead = if_nameindex();
	if (phead == NULL) {
		perror("if_nameindex:");
		exit(EXIT_FAILURE);
	}

	pitr = phead;
	while(1) {
		if (isLast(pitr)) {
			break;
		}
		printf("%d name:%s\n", pitr->if_index, pitr->if_name);
		pitr++;
	}

	// should call for [phead] not for [pitr]
	if_freenameindex(phead);

	return 0;
}