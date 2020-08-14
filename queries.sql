$ QUERIES
$ --------------------------------------------------------------------
$ Ej_01
SELECT
    c.client_id,
    c.name,
    SUM(IF(b.date_of_issue < '2020-01-01', 1, 0)) AS num_bills
FROM
    client c
        INNER JOIN
    bill b USING(client_id)
GROUP BY c.client_id;

$ Ej_02
SELECT
    b.bill_id,
    b.date_of_issue,
    SUM(bl.quantity * bl.unit_price) AS import
FROM
    bill b
        INNER JOIN
    bill_lines bl USING(bill_id)
GROUP BY b.bill_id;

$ Ej_03
SELECT
    v.vendor_id,
    v.name,
    SUM(
        IF(b.date_of_issue < '2020-01-01',
            (bl.quantity * (bl.unit_price - bl.discount)) - b.global_discount,
            0
        )
    ) AS total_import_2019
FROM
    (
        vendor v
            INNER JOIN
        bill b USING(vendor_id)
    )
        INNER JOIN
    bill_lines bl USING(bill_id)
GROUP BY v.vendor_id;

$ Ej_04
SELECT
    p.name,
    SUM(IFNULL(bl.quantity, 0))
FROM
    bill_lines bl
        RIGHT JOIN
    (
        bill b
            RIGHT JOIN
        (
            vendor v
                RIGHT JOIN
            (
                town t
                    RIGHT JOIN
                province p USING(province_id)
            ) USING(town_id)
        ) USING(vendor_id)
    ) USING(bill_id)
GROUP BY(p.province_id)

$ Ej_05
SELECT
    c.client_id,
    c.name
FROM
(
    vendor v
        INNER JOIN
    (
        town tv
            INNER JOIN
        province pv USING(province_id)
    ) USING(town_id)
)
    INNER JOIN
(
    bill b
        INNER JOIN
    (
        client c
            INNER JOIN
        (
            town tc
                INNER JOIN
            province pc USING(province_id)
        ) USING(town_id)
    ) USING(client_id)
) USING(vendor_id)
WHERE
    pv.province_id <> pc.province_id
GROUP BY(c.client_id);

$ Ej_06
SELECT
    c.client_id,
    c.name
FROM
    bill_lines bl
        INNER JOIN
    (
        bill b
            INNER JOIN
        (
            client c
                INNER JOIN
            (
                town t
                    INNER JOIN
                province p USING(province_id)
            ) USING(town_id)
        ) USING(client_id)
    ) USING(bill_id)
GROUP BY bl.bill_id
HAVING COUNT(*) >= 10;

$ Ej_08
SELECT
    c.client_id,
    c.name,
    IFNULL(SUM((bl.quantity * (bl.unit_price - bl.discount))
                - b.global_discount), 0) AS total_billing
FROM
    (
        client c
            LEFT JOIN
        bill b USING(client_id)
    )
        LEFT JOIN
    bill_lines bl USING(bill_id)
GROUP BY c.client_id
HAVING total_billing > 6000;
$ --------------------------------------------------------------------
