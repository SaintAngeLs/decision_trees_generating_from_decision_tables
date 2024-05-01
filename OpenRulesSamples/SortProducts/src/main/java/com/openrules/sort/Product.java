package com.openrules.sort;

public class Product implements Comparable<Product> {
	
	String name;
	boolean hasFeature1;
	boolean hasFeature2;
	boolean hasFeature3;
	int customerScore;
	boolean eligible;
	
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	
	public boolean isHasFeature1() {
		return hasFeature1;
	}
	public void setHasFeature1(boolean hasFeature1) {
		this.hasFeature1 = hasFeature1;
	}
	public boolean isHasFeature2() {
		return hasFeature2;
	}
	public void setHasFeature2(boolean hasFeature2) {
		this.hasFeature2 = hasFeature2;
	}
	public boolean isHasFeature3() {
		return hasFeature3;
	}
	public void setHasFeature3(boolean hasFeature3) {
		this.hasFeature3 = hasFeature3;
	}
	public int getCustomerScore() {
		return customerScore;
	}
	public void setCustomerScore(int customerScore) {
		this.customerScore = customerScore;
	}
	
	public boolean isEligible() {
		return eligible;
	}
	public void setEligible(boolean eligible) {
		this.eligible = eligible;
	}
	
	public int compareTo(Product product) {
		if(product.customerScore > this.customerScore)
			return 1;
		else 
			return -1;
	}
	@Override
	public String toString() {
		return "Product [name=" + name + ", hasFeature1=" + hasFeature1 + ", hasFeature2=" + hasFeature2
				+ ", hasFeature3=" + hasFeature3 + ", customerScore=" + customerScore + ", eligible=" + eligible + "]";
	}
	
	

}
