/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>
	Copyright (C) 2009-2010  Daniel Wonisch <dwonisch@campus.uni-paderborn.de>

	RobotSwarmSimulator is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	RobotSwarmSimulator is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with RobotSwarmSimulator.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 *  aiee.h
 *  Robot Swarm Simulator
 *
 *  Created by Peter Kling on 28.04.09.
 */

#include <Utilities/lapack.h>


#ifndef USE_LAPACK // if LAPACK not available, don't include the full easter egg
void aiee() {
	std::cout << "You found 'AIEE', but you seem not to have a working LAPACK environment :-/" << std::endl;
	std::cout << "If you want to know what 'AIEE' is, install/setup LAPACK and come back!" << std::endl;
}
#else // LAPACK available --> go on

void aiee() {
	std::cout << "AIEE - Welcome to the 'Another Informative Easter Egg'" << std::endl;
	std::cout << "=====================================================" << std::endl << std::endl;
	
	
	/* ----- stolen lapack test (from http://hep.ph.liv.ac.uk/~hock/My_reports/CLAPACK/CLAPACK_notes.htm) ----- */
	
	integer  mmax = 10, nb = 64,     nmax = 8;
	integer lda = mmax, ldvt = nmax, lwork = mmax+4*nmax+nb*(mmax+nmax);
	integer ldu = 1;
	integer  i, info, j, m, n;
	double a[10*10],  dummy[1*1], s[8], vt[8*8], work[10+4*8+64*(10+8)];
	
	// input matrix (column major ordering)
	m=6; n=4;
	a[0+0*lda] = 2.27;
	a[0+1*lda] = -1.54;
	a[0+2*lda] = 1.15;
	a[0+3*lda] = -1.94;
	
	a[1+0*lda] = 0.28;
	a[1+1*lda] = -1.67;
	a[1+2*lda] = 0.94;
	a[1+3*lda] = -0.78;
	
	a[2+0*lda] = -0.48;
	a[2+1*lda] = -3.09;
	a[2+2*lda] = 0.99;
	a[2+3*lda] = -0.21;
	
	a[3+0*lda] = 1.07;
	a[3+1*lda] = 1.22;
	a[3+2*lda] = 0.79;
	a[3+3*lda] = 0.63;
	
	a[4+0*lda] = -2.35;
	a[4+1*lda] = 2.93;
	a[4+2*lda] = -1.45;
	a[4+3*lda] = 2.30;
	
	a[5+0*lda] = 0.62;
	a[5+1*lda] = -7.39;
	a[5+2*lda] = 1.03;
	a[5+3*lda] = -2.57;
	
	// print input matrix
	std::cout << std::endl << "Original matrix A" << std::endl << std::endl;
	std::cout << "Dimension: " << m << "\t" << n << std::endl << std::endl;
	for (i=0; i<m; i++) {
		for (j=0; j<n; j++) printf("%lf    ", a[i+j*lda]);
		printf("\n");
	}

	// computation of singular value decomposition of the matrix
	dgesvd_((char*)"O", (char*)"S", &m, &n, a, &lda, s, dummy, &ldu, vt, &ldvt, work, &lwork, &info);
	
	// print the results		
	printf("\nDGESVD Example Program Results\n\n");	
	printf("\nSingular values\n\n");
	for (j=0; j<n; j++) printf("%lf    ", s[j]);
	
	printf("\n\nLeft singular vectors (first n columns of U)\n\n");
	for (i=0; i<m; i++) {
		for (j=0; j<n; j++) printf("%lf    ", a[i+j*lda]);
		printf("\n");
	}
	
	printf("\nRight singular vectors by row (V**T)\n\n");
	for (i=0; i<n; i++) {
		for (j=0; j<n; j++) printf("%lf    ", vt[i+j*ldvt]);
		printf("\n");
	}
	
	
	printf("\nIf the output above looks simillar (i.e. modulo signs and accuracy) to ");
	printf("http://hep.ph.liv.ac.uk/~hock/My_reports/CLAPACK/dgesvd-ex.r, everything is fine :-).\n\n");
	
	printf("...now guess what we computed above ;-).\n");
}

#endif /* USE_LAPACK */
