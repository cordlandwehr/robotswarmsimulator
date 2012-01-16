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
public class StrategyCController extends SimulationController {

	public StrategyCController(Input input) {
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

		//let source and dest meet in the middle of the two
		while (Math.abs(posS-posD) > 1) {
			posS = moveNode(posS, dir);
			
			if (Math.abs(posS-posD) > 1)
				posD = moveNode(posD, -dir);
		}
	}

	@Override
	public String getName() {
		return "StrategyC";
	}

}
