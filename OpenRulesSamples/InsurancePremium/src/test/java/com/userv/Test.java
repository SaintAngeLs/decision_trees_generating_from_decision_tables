/*******************************************************************************
 * Copyright (c) 2019 OpenRules, Inc.
 *******************************************************************************/
package com.userv;

import java.util.Map;

import com.openrules.core.util.CLIParser;

public class Test {

	public static void main(String[] args) throws Exception {
	    Map<String,String> properties = new CLIParser().parse(args).getOptions();
		DecisionModelInsurancePremiumTest test = new DecisionModelInsurancePremiumTest();
		test.runTests(properties);
	}
}
