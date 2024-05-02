package com.openrules.spatial;

import java.util.ArrayList;
import java.util.List;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.openrules.core.*;
import com.openrules.core.event.*;
import com.openrules.core.util.*;
import com.openrules.types.*;

import com.vividsolutions.jts.geom.Geometry;

public class EntityRepository {
    
    private static Logger Log = LoggerFactory.getLogger(EntityRepository.class);
	
	static County[] counties = null;
	static HospitalReferralRegion[] hrrs = null;
	static HospitalServiceArea[] hsas = null;
	static Hospital[] hospitals = null;
	
	static void build() {
		// Build GeoDatabase from GML files
		if (!GeoDatabaseBuilder.build()) {
			Log.error("Cannot build GeoDatabase");
			System.exit(-1);
		}
		
		int n = GeoDatabase.countyGeometries.size();
		counties = new County[n];
		for (int i = 0; i < n; i++) {
			counties[i] = new County(i,GeoDatabase.countyGeometries.get(i));
		}
		
		n = GeoDatabase.hrrGeometries.size();
		hrrs = new HospitalReferralRegion[n];
		for (int i = 0; i < n; i++) {
			hrrs[i] = new HospitalReferralRegion(i,GeoDatabase.hrrGeometries.get(i));
		}
		
		n = GeoDatabase.hsaGeometries.size();
		hsas = new HospitalServiceArea[n];
		for (int i = 0; i < n; i++) {
			hsas[i] = new HospitalServiceArea(i,GeoDatabase.hsaGeometries.get(i));
		}
		
		n = GeoDatabase.hospitalGeometries.size();
		hospitals = new Hospital[n];
		for (int i = 0; i < n; i++) {
			hospitals[i] = new Hospital(i,GeoDatabase.hospitalGeometries.get(i));
		}
		
	}

}
