/**
 * 
 */
package nodeSim.controllers;

import nodeSim.SimulationController;
import nodeSim.containers.Input;

/**
 * This is strategy C as described in the thesis.
 *
 */
public class StrategyGTTMController extends SimulationController {
	int[][] knownNeighbors; //[node][0..1]	

	public StrategyGTTMController(Input input) {
		super(input);
		knownNeighbors = new int[input.getNumberOfNodes()][2];
		for (int i = 0; i < input.getNumberOfNodes(); i++) {
			knownNeighbors[i][0] = -1;
			knownNeighbors[i][1] = -1;
		}
 			
	}

	@Override
	protected void rearrange(int posS, int posD) {
		int idS = nodes[posS].getOptimalPosition();
		int idD = nodes[posD].getOptimalPosition();
		
		if (knownNeighbors[idS][0] < 0) {
			knownNeighbors[idS][0] = idD;
		}
		else if (knownNeighbors[idS][0] != idD) {
			knownNeighbors[idS][1] = idD;
		}
		
		if (knownNeighbors[idD][0] < 0) {
			knownNeighbors[idD][0] = idS;
		}
		else if (knownNeighbors[idD][0] != idS) {
			knownNeighbors[idD][1] = idS;
		}		
		
		
		boolean allNeighborsKnown = (knownNeighbors[idS][0] >= 0 && knownNeighbors[idS][1] >= 0);
		//System.out.println("idS: "+idS+", idD: "+idD);
		//System.out.println("s: "+idS+", [0]: "+knownNeighbors[idS][0] + ", [1]: "+knownNeighbors[idS][1]);
		
		if (allNeighborsKnown) { //otherwise, don't move at all
			//idON = id of the other neighbor
			int idON = (idD == knownNeighbors[idS][0]) ? knownNeighbors[idS][1] : knownNeighbors[idS][0];
			int posON = getCurrentPosition(idON);
			
			if ((posON < posS && posS < posD) || (posD < posS && posS < posON)) { //we are in the middle already
				//do nothing
			}
			else if (posON > posS && posD > posS) { //both nodes are right of us
				while( posS < posD && posS < posD ) {
					posS = moveNode(posS, 1); //move to the right
				}
			} 
			else if (posON < posS && posD < posS) { //both nodes are left of us
				while( posS > posD && posS > posD ) {
					posS = moveNode(posS, -1); //move to the left
				}				
			}
		}
	}

	@Override
	public String getName() {
		return "StrategyGTTM";
	}

}
