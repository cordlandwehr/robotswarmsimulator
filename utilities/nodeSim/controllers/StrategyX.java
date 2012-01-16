/**
 * 
 */
package nodeSim.controllers;

import nodeSim.SimulationController;
import nodeSim.containers.Input;

/**
 * A dummy strategy that does nothing at all.
 *
 */
public class StrategyX extends SimulationController {

	public StrategyX(Input input) {
		super(input);
	}

	@Override
	protected void rearrange(int posS, int posD) {
		//do nothing at all
	}

	@Override
	public String getName() {
		return "StrategyX";
	}

}
