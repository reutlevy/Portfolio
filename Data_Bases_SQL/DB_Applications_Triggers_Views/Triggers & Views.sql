CREATE VIEW ConstructorEmployeeOverFifty AS
SELECT Employee.*,ConstructorEmployee.CompanyName,ConstructorEmployee.SalaryPerDay 
FROM (Employee JOIN ConstructorEmployee 
ON Employee.EID = ConstructorEmployee.EID AND (date('now')-Employee.BirthDate)>=50);

CREATE VIEW ApartmentNumberInNeighborhood AS
SELECT Neighborhood.NID, ApartmentNumber FROM Neighborhood LEFT JOIN 
(SELECT Apartment.NID, COUNT(NID) as ApartmentNumber FROM Apartment GROUP BY NID) as rea1 ON Neighborhood.NID = rea1.NID;


CREATE TRIGGER DeleteProject
After DELETE on Project
BEGIN 
DELETE FROM ProjectConstructorEmployee WHERE ProjectConstructorEmployee.PID = old.PID;
DELETE FROM ConstructorEmployee WHERE EID not in (SELECT EID FROM ProjectConstructorEmployee);
DELETE FROM Employee WHERE EID not in (SELECT EID FROM ConstructorEmployee); END;


CREATE TRIGGER MaxManger
BEFORE INSERT ON Department 
BEGIN
	SELECT CASE
	WHEN(SELECT counter FROM (SELECT COUNT(ManagerID) as counter from Department GROUP BY ManagerID) HAVING ManagerID = new.ManagerID)> 1
	THEN RAISE (ABORT , 'Employee cant manage more than 2 Departments')
	END;
end;	
	
CREATE TRIGGER MaxManger1
BEFORE UPDATE ON Department 
BEGIN
	SELECT CASE
	WHEN(SELECT counter FROM (SELECT COUNT(ManagerID) as counter from Department GROUP BY ManagerID) HAVING ManagerID = new.ManagerID)> 1
	THEN RAISE (ABORT , 'Employee cant manage more than 2 Departments')
	END;
end;	
		
-- Add Triggers Here, do not forget to separate the triggers with ;