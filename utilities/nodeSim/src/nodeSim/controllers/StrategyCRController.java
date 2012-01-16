/**
 * 
 */
package nodeSim.controllers;

import nodeSim.SimulationController;
import nodeSim.containers.Input;

/**
 * This is the randomized version of strategy C (C') as described in the thesis.
 * @author asetzer
 *
 */
public class StrategyCRController extends SimulationController {

	public StrategyCRController(Input input) {
		super(input);
	}

	@Override
	protected void rearrange(int posS, int posD) {
		int dir = 0;
		//if source is left to dest, then move source to the right - else move it to the left 
		if (posS < posD)
			dir = 1;
		else
			dir = -1;
		
		if (posS + dir == posD)
			return;
		
		//distance is 2
		if (posS + dir*2 == posD) {
			if ( Math.random() >= 0.5 )
				posS = moveNode(posS, dir);
			else
				posD = moveNode(posD, -dir);
		}
		else { //distance is greater than 2
			if ( Math.random() >= 0.5 )
				posS = moveNode(posS, dir*2);
			else
				posD = moveNode(posD, -dir*2);
		}
		
		//let source and dest meet in the middle of the two
		while (Math.abs(posS-posD) > 1) {
			posS = moveNode(posS, dir);
			
			if (Math.abs(posS-posD) > 1)
				posD = moveNode(posD, -dir);
		}
	}

	@Override
	public String getName() {
		return "StrategyCR";
	}

}
