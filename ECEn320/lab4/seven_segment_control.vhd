
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity seven_segment_control is
	generic(
		COUNTER_BITS: natural := 15
		);
    Port ( clk : in  STD_LOGIC;
           data_in : in  STD_LOGIC_VECTOR (15 downto 0);
           dp_in : in  STD_LOGIC_VECTOR (3 downto 0);
           blank : in  STD_LOGIC_VECTOR (3 downto 0);
           seg : out  STD_LOGIC_VECTOR (6 downto 0);
           dp : out  STD_LOGIC;
           an : out  STD_LOGIC_VECTOR (3 downto 0));
end seven_segment_control;

architecture Behavioral of seven_segment_control is
	signal r_reg, r_next : unsigned (COUNTER_BITS - 1 downto 0) := (others => '0');
	signal dataMuxOutput : STD_LOGIC_VECTOR(3 downto 0);
	signal anode_select : unsigned(1 downto 0);
begin

	-- register
	process(clk)
	begin
		if (clk'event and clk='0') then
			r_reg <= r_next;
		end if;
	end process;
	
	-- free binary counter
	r_next <= r_reg + 1;
		
	-- anode logic
	anode_select <= r_reg(COUNTER_BITS - 1 downto COUNTER_BITS - 2);
	
	an <= "111" & blank(0) when (anode_select = "00") else
			"11" & blank(1) & '1' when (anode_select = "01") else
			'1' & blank(2) & "11" when (anode_select = "10") else
			blank(3) & "111";
			
	-- data select
	with anode_select select
		dataMuxOutput <= data_in(3 downto 0) when "00",
			data_in(7 downto 4) when "01",
			data_in(11 downto 8) when "10",
			data_in(15 downto 12) when others;
	
	-- seven segment decoder
	with dataMuxOutput select
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
				
	-- dp logic
	with anode_select select
		dp <= not dp_in(0) when "00",
			not dp_in(1) when "01",
			not dp_in(2) when "10",
			not dp_in(3) when others;

end Behavioral;

