/*******************************************************************************
 * Copyright (c) 2019 OpenRules, Inc.
 *******************************************************************************/
package aggregated.values.lists;

import java.util.Map;

import com.openrules.core.util.CLIParser;

public class Main {
	
	public static void main(String[] args) throws Exception {
	    Map<String,String> properties = new CLIParser().parse(args).getOptions();
		
	    DecisionModelAggregatedValuesTest test = new DecisionModelAggregatedValuesTest();
		test.runTests(properties);
	}
}
