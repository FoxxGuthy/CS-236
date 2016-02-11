----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    14:37:53 01/21/2016 
-- Design Name: 
-- Module Name:    seven_seg_decode - seven_seg_arch 
-- Project Name: 
-- Target Devices: 
-- Tool versions: 
-- Description: 
--
-- Dependencies: 
--
-- Revision: 
-- Revision 0.01 - File Created
-- Additional Comments: 
--
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity seven_seg_decode is
    Port ( sw : in  STD_LOGIC_VECTOR (7 downto 0);
           btn : in  STD_LOGIC_VECTOR (3 downto 0);
           seg : out  STD_LOGIC_VECTOR (6 downto 0);
           dp : out  STD_LOGIC;
           an : out  STD_LOGIC_VECTOR (3 downto 0));
end seven_seg_decode;

architecture seven_seg_arch of seven_seg_decode is
	signal mux1output : STD_LOGIC_VECTOR (3 downto 0);
	signal mux2output : STD_LOGIC_VECTOR (3 downto 0);
begin
	with mux2output select
		seg <= "1000000" when "0000", --0
				"1111001" when "0001", --1
				"0100100" when "0010", --2
				"0110000" when "0011", --3
				"0011001" when "0100", --4
				"0010010" when "0101", --5
				"0000010" when "0110", --6
				"1111000" when "0111", --7
				"0000000" when "1000", --8
				"0010000" when "1001", --9
				"0001000" when "1010", --A
				"0000011" when "1011", --B
				"1000110" when "1100", --C
				"0100001" when "1101", --D
				"0000110" when "1110", --E
				"0001110" when others; --F
	
	--Display all when btn3 is pressed
	mux2output <= "1000" when (btn(3) = '1') else
			mux1output;

	dp <= '0' when (btn(3) = '1') else --more low assertions
			'1';
							
	--btn 1 and 0 logic
	mux1output <= sw(3 downto 0) when (btn(1 downto 0) = "00") else
						sw(7 downto 4) when (btn(1 downto 0) = "01") else
						sw(3 downto 0) xor sw(7 downto 4) when (btn(1 downto 0) = "10") else
						sw(1) & sw(0) & sw(3) & sw(2) when (btn(1 downto 0) = "11") else
						mux1output;
						
	an <= "0000" when (btn(3) = '1') else
			"1111" when (btn(2) = '1') else
			"1110" when (btn(1 downto 0) = "00") else
			"1101" when (btn(1 downto 0) = "01") else
			"1011" when (btn(1 downto 0) = "10") else
			"0111";
					
end seven_seg_arch;

