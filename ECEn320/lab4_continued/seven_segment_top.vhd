----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    16:24:58 01/28/2016 
-- Design Name: 
-- Module Name:    seven_segment_top - Behavioral 
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
use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity seven_segment_top is
    Port ( clk : in  STD_LOGIC;
           btn : in  STD_LOGIC_VECTOR (3 downto 0);
           sw : in  STD_LOGIC_VECTOR (7 downto 0);
           seg : out  STD_LOGIC_VECTOR (6 downto 0);
           dp : out  STD_LOGIC;
           an : out  STD_LOGIC_VECTOR (3 downto 0));
end seven_segment_top;

architecture Behavioral of seven_segment_top is

	-- signals
	signal data_in_w : std_logic_vector (15 downto 0);
	signal dp_in_w : std_logic_vector (3 downto 0);
	signal blank_w : std_logic_vector (3 downto 0);
	signal counter, counter_next : unsigned (31 downto 0) := (others => '0');

	component seven_segment_control
		generic (COUNTER_BITS: natural := 15);
		port(
			clk: in STD_LOGIC;
			data_in: in STD_LOGIC_vector (15 downto 0);
			dp_in: in std_logic_vector (3 downto 0);
			blank: in std_Logic_vector (3 downto 0);
			seg : out std_logic_vector (6 downto 0);
			dp : out std_logic;
			an : out std_logic_vector (3 downto 0)
			);
	end component;
	
begin
	-- creat instaniaiton
	sev_seg : seven_segment_control
		Port map (
		clk => clk, 
					data_in => data_in_w,
					dp_in => dp_in_w,
					blank => blank_w,
					seg => seg, 
					dp => dp, 
					an => an
					);
					
	--register
		process(clk)
	begin
		if (clk'event and clk='0') then
			counter <= counter_next;
		end if;
	end process;
	
	--counter
	counter_next <= counter + 1;
	
	-- BTN LOGIC
	blank_w <= "1111" when btn(3) = '1' else
				"0000" when btn(2) = '1' else
				"1100" when btn(1) = '1' else
				"0000"; -- don't blank any
				
	data_in_w <= std_logic_Vector(counter(15 downto 0)) when btn(0) = '1' else
					"00000000" & sw when btn(1) = '1' else -- SWITCH STUFF
					 "1011111011101111" when btn(2) = '1' else -- BEEF
				std_logic_Vector(counter(31 downto 16));
				
	dp_in_w <= "0000" when btn(3) = '1' else -- all on
					"1111" when btn(2) = '1' else -- all off
					"1101" when btn(1) = '1' else
					"0000" when btn(0) = '1' else -- all on
					"0111";
	
	
end Behavioral;

