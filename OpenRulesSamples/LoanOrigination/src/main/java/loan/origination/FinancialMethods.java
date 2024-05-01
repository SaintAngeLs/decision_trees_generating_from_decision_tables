/*******************************************************************************
 * Copyright (c) 2019 OpenRules, Inc.
 *******************************************************************************/
package loan.origination;

//import org.apache.poi.ss.formula.functions.FinanceLib;

public class FinancialMethods {
	
//	public static double calc(double rate, double term, double amount) {
//		double r = rate/12;
//		double t = (term/12)*12;
//		return FinanceLib.pmt(r, t, -amount, 0, false);
//	}
	
	public static double pmt(double rate, double term, double amount) {
		double r = rate/12;
		double t = (term/12)*12;
		return (amount*r)/(1 - Math.pow(1+r,-t));
	}

	public static void main(String[] args) {
		
		System.out.println("pmt: " + FinancialMethods.pmt(0.10, 36, 100000));
		
    }

}
